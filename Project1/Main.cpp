//#include"MCTSNode.hpp"
//#include"ConnectFour.hpp"
//int main() {
//	ConnectFour t;
//	/*
//	t.pvp();
//	t.pve(true);
//	t.pve(false);*/
//	t.AI_course_socket();
//	return 0;
//}
/*
dev-cpp����
libwsock32.a

vs����
_CRT_SECURE_NO_WARNINGS
_WINSOCK_DEPRECATED_NO_WARNINGS

cmake����
link_libraries(ws2_32)
*/
#include"AlphaBetaPruning.hpp"
int main() {
	AlphaBetaPruning t;
	//t.pvp();
	//������
	//t.pve(true);
	//�˺���
	t.pve(false);
	//t.AI_course_socket();
	return 0;
}