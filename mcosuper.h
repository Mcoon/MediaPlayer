#ifndef MCOSUPER_H
#define MCOSUPER_H

class McoSuper
{
private:
    McoSuper();
	
public:
    static  McoSuper* getObject();
	/*
	* ��ȡ��ǰʱ��
	* return ����
	*/
	static long long getNowTime();
};

#endif // MCOSUPER_H
