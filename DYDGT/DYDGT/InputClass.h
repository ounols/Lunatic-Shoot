#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_



class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	//입력 초기화
	void Initialize();

	//눌렀을 때의 함수
	void KeyDown(unsigned int);
	//땠을 때의 함수
	void KeyUp(unsigned int);

	//어느 키가 눌려저 있는지 확인
	bool IsKeyDown(unsigned int);


private:
	//입력된 키
	bool m_keys[256];


};


#endif