#include "InputClass.h"


InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

void InputClass::Initialize(){
	//��� Ű�� �ʱ�ȭ
	for (int i = 0; i < 256; i++){
		m_keys[i] = false;
	}

	return;


}


void InputClass::KeyDown(unsigned int input){
	m_keys[input] = true;

	return;


}

void InputClass::KeyUp(unsigned int output){
	m_keys[output] = false;

	return;


}


bool InputClass::IsKeyDown(unsigned int key){
	return m_keys[key];


}
