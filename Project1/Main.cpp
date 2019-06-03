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
dev-cpp加入
libwsock32.a

vs加入
_CRT_SECURE_NO_WARNINGS
_WINSOCK_DEPRECATED_NO_WARNINGS

cmake加入
link_libraries(ws2_32)
*/
#include"AlphaBetaPruning.hpp"
int main() {
	AlphaBetaPruning t;
	//t.pvp();
	//人先手
	//t.pve(true);
	//人后手
	t.pve(false);
	//t.AI_course_socket();
	return 0;
}