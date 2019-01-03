#pragma once

/*
* ��������ͣ�����������ټ�˵��
* @date : 2018/10/23
* @author : jihang
*/

#include <QString>

class ControlCode {

private:
	//����ط���Ӧ�˶����������
	static QString TYPE_PAUSE;
	static QString TYPE_CONTINUE;
	static QString TYPE_END;

public:
	static QString getPauseType() {
		return TYPE_PAUSE;
	}
	static QString getContinueType() {
		return TYPE_CONTINUE;
	}
	static QString getEndType() {
		return TYPE_END;
	}
};