#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_



class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	//�Է� �ʱ�ȭ
	void Initialize();

	//������ ���� �Լ�
	void KeyDown(unsigned int);
	//���� ���� �Լ�
	void KeyUp(unsigned int);

	//��� Ű�� ������ �ִ��� Ȯ��
	bool IsKeyDown(unsigned int);


private:
	//�Էµ� Ű
	bool m_keys[256];


};


#endif