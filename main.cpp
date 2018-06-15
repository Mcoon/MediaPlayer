#include "McoAVPlay01.h"
#include <QtWidgets/QApplication>
extern "C"
{
#include <libavformat/avformat.h>
}
#pragma comment(lib,"avformat.lib")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	av_register_all();
	McoAVPlay01 w;
	w.show();
	return a.exec();
}
