#pragma once

#include <array>
#include <atomic>
#include <thread>

#include "Constants.h"

// LMAX Disruptor 기반 동기화 버퍼
// MPSC Ring Buffer

// 핵심 설계 기준
// 
// 1. 메모리 할당
//  - 링 버퍼의 모든 메모리는 시작 시 미리 할당된다 -> 런타임에 memory allocation 없음
// 
// 2. 기존 큐의 문제점
//  - 연결 리스트 기반 큐는 Cache Locality가 매우 안 좋다
// 
// 3. 인덱스 계산
//  - 대부분의 자료구조는 시퀀스 번호에서 나머지 계산을 해서 인덱스 계산 -> 너무 무거움
//  - 링의 크기를 2의 거듭제곱으로 만들고 (size - 1)의 비트 마스크를 사용해서 최적화
//
// 4. 시퀀싱
//  - 동기화를 위해 전역 시퀀스를 사용해서 링버퍼의 슬롯을 순서대로 접근한다
//  - (멀티 Producer의 경우) atomic increment or CAS로 다음 시퀀스 확보 -> Data Write
//  - Producer는 Data Write가 끝나면 커서를 갱신해 Consumer에게 공개
//  - Consumer는 cursor가 자신이 기다리는 시퀀스까지 올라올 때까지 기다린다
//  - Consumer도 자기만의 시퀀스 커서를 가진다

// 클래스 구성 및 설명
//
// 1. RingBuffer
//  - 고정 크기 객체 배열 관리
//  - 가장 최근에 쓰인 시퀀스를 나타내는 cursor 관리
// 
//  - claim() : 다음 쓸 슬롯의 시퀀스 확보
//  - publish() : 데이터를 쓴 후 cursor 갱신
//  - waitFor() : 특정 시퀀스가 소비 가능할 때까지 대기
// 
// 2. Entry & EntryFactory
//  - Entry : RingBuffer에 들어갈 객체
//  - EntryFactory : 객체 풀
// 
// 3. ProducerBarrier
//  - 쓰기 가능한 시퀀스 확보 / 데이터를 쓴 후 커서 전진
//  
// 4. ConsumerBarrier
//  - 읽기 가능한 시퀀스 확보 / 확보 못하면 대기
// 
// 5. Consumer
//  - 소비자 객체
//  - 각 Consumer는 자신만의 시퀀스 보유
// 
// 6. ClaimStrategy
//  - 쓰기 가능한 시퀀스 확보 방법 정의하는 인터페이스
//  - Multi Producer의 경우 CAS를 사용하여 시퀀스 확보
// 
// 7. WaitStrategy
//  - Consumer의 대기 정책 정의하는 인터페이스
//  - Busy-Wait / Yield / Condition-Variable / Semapore etc...
// 
// 8. BatchHandler
//  - Consumer가 이벤트를 받아 처리할 때 실행되는 Callback 인터페이스
// 
//  - onAvailable(Entry entry) : 새 데이터가 도착할 때 호출
//  - onEndOfBatch() : 일괄 처리 끝날 때 호출
//  - onCompletion() : 종료 전 정리 작업