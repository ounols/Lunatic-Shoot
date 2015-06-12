#include "TimerClass.h"


TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass& other){

}


TimerClass::~TimerClass()
{
}


bool TimerClass::Initialize(){
	//시스템이 하이 퍼포먼스 타이머를 지원하는지 체크합니다.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0){
		return false;
	}

	//매 밀리초로 카운트 타이머가 얼마나 시간이 지나갔는지 변환.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;

}

void TimerClass::Frame(){
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeDifference = (float)(currentTime - m_startTime);
	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime(){
	return m_frameTime;
}