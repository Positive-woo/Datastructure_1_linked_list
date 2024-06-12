#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct Listdata {
	char name[30];
	int id;
	char phone[20];
	char province[30];
	float gpa;
} listData;

// 단순연결리스트의 노드 구조를 구조체로 정의
typedef struct {
	listData data;
	struct ListNode* link;
}listNode;

// 리스트 시작을 나타내는 head 노드를 구조체로 정의
typedef struct LinkedList {
	listNode* head;
	int length;
}linkedList;

linkedList* initList() {
	linkedList* L;
	L = (linkedList*)malloc(sizeof(linkedList));
	L->head = NULL;
	L->length = 0;
	return L;
}

int getLength(linkedList* L) {
	return L->length;
}

void insert(linkedList* L, listNode* pre, listData x) {
	//빈 노드 먼저 생성
	listNode* newNode;
	newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data = x;

	//case 1) list가 비어있는 경우
	if (L->head == NULL) {
		newNode->link = NULL;
		L->head = newNode;
	}
	//case 2) list의 첫번째에 넣는 경우
	else if (pre == NULL) {
		newNode->link = L->head;
		L->head = newNode;
	}
	//case 3) 일반적으로 사용되는 경우 (중간)
	else {
		newNode->link = pre->link;
		pre->link = newNode;
	}
	//if문들 탈출하면 L길이 하나 늘리기
	L->length++;
}

void insertFirst(linkedList* L, listData x) {
	listNode* newNode;
	newNode = (listNode*)malloc(sizeof(listNode));
	// newNode의 data값과 link값 넣기!! 위에 case 2 참고
	newNode->data = x;
	newNode->link = L->head;
	L->head = newNode;
	L->length++;
}

void insertLast(linkedList* L, listData x) {
	listNode* newNode, * temp;
	newNode = (listNode*)malloc(sizeof(listNode));
	// newNode의 data 와 link 지정 마지막에 넣는거 고정이니까 link = NULL
	newNode->data = x;
	newNode->link = NULL;

	//case 1) list가 비어있는 경우
	if (L->head == NULL) {
		L->head = newNode;
	}
	//case 2) 일반적인 경우
	else {
		temp = L->head;
		//temp가 끝(NULL)에 도달하지 않는다면 앞으로 앞으로~
		while (temp->link != NULL)
			temp = temp->link;
		temp->link = newNode;
	}
	L->length++;
}


int delete(linkedList* L, listNode* p) {
	listNode* pre;

	// case 1) list가 비어있는 경우
	if (L->head == NULL) return FALSE;
	// case 2) 찾는 값이 list에 없는 경우
	if (p == NULL) return FALSE;
	// case 3) 찾는 값이 첫번째인 경우 > head를 p link로 옮기고 비워주기
	if (L->head == p) {
		L->head = p->link;
		free(p);
	}
	else {// case 4) 찾는 값이 첫번째가 아닌 경우 > 일반적인 경우
		pre = L->head;
		while (pre->link != p) {//이전 노드가 향하고 있는 것이 p가 아니라면 앞으로 전진
			pre = pre->link;
			if (pre == NULL) return FALSE; //NULL 즉 끝에 도달하면 FALSE --> CASE 2랑 같은거 아닌가?
		}
		pre->link = p->link;
		free(p);
	}
	L->length--;
	return TRUE;
}

listNode* search(linkedList* L, int x) {
	//temp를 설정 후 head로 정의
	listNode* temp = L->head;
	// int x값을 id라고 가정하고 설정
	while (temp != NULL) { //찾으면 return 못찾으면 전진
		if (temp->data.id == x)
			return temp;
		else
			temp = temp->link;
	}
	return temp; //이건 NULL이라면 NULL값 출력하라고 있는건가?
}

void displayList(linkedList* L) {
	listNode* p;
	printf("L=(");
	p = L->head;
	while (p != NULL) {
		printf("[%s %d %s %s %.2f]\n", p->data.name, p->data.id, p->data.phone, p->data.province, p->data.gpa);
		p = p->link;
		if (p != NULL) printf("   ");
	}
	printf(")\n");
}

void clear(linkedList* L) {
	// free 써야되는거같음
	listNode* temp = L->head;
	listNode* link_bridge; //temp의 다음 다리 연결용

	if (temp != NULL) {
		link_bridge = temp->link; //중간 다리에 temp_link값 잠시 넣어두고
		free(temp);				  //temp free 시키고
		temp = link_bridge;		  //temp에 아까 보관해둔 다리 다시 부르기
	}
	L->length = 0;
}

void reverse(linkedList* L) {
	listNode *prev, *curr, *next; //prev 전꺼 가리키기, curr 중간꺼, next 다음꺼 길 잃지않게 미리 보내기
	prev = NULL;
	curr = L->head;

	while (curr != NULL) {
		next = curr->link; 
		curr->link = prev;
		prev = curr;
		curr = next;
	}
	L->head = prev;
}




int main() {
	linkedList* L;
	listNode* p;

	// 공백 리스트 생성
	L = initList();

	// (과제 2) 파일을 읽어 Linked list 데이터 삽입
	// 출처: https://blockdmask.tistory.com/392  코드 참고
	FILE* student_pFile= fopen("student.txt", "r");
	if(student_pFile==NULL){
		printf("파일이 없습니다. 프로그램을 종료합니다.");
		return 0;
	}
	else {
		printf("student파일을 읽었습니다.\n");
	}
	FILE* gpa_pFile = fopen("gpa.txt", "r");
	if (gpa_pFile== NULL) {
		printf("파일이 없습니다. 프로그램을 종료합니다.");
		return 0;
	}
	else {
		printf("gpa파일을 읽었습니다.\n");
	}

	
	// student, gpa 파일 받아와서 linked list에 넣어야겠지?
	while(!feof(student_pFile) && !feof(gpa_pFile)) { //feof파일 끝에 도달 > !feof 파일끝 도달이 아님 > 두 파일 다 끝 도달이 아니라면 계속 진행
		char tmp_name[30];
		int tmp_id=0;
		char tmp_phone[20];
		char tmp_province[30];
		float tmp_gpa;

		fscanf(student_pFile, "%s %d %s %s", tmp_name, &tmp_id, tmp_phone, tmp_province);
		fscanf(gpa_pFile, "%f", &tmp_gpa);
		
		listData newData;
		strcpy(newData.name, tmp_name);
		newData.id = tmp_id;
		strcpy(newData.phone, tmp_phone);
		strcpy(newData.province, tmp_province);
		newData.gpa = tmp_gpa;
		insertLast(L, newData);
		
	}
	fclose(student_pFile);
	
	while (1) {
		char buf[500];
		char* input_id1;
		char* input_id2;
		char* input_name1;
		char* input_phone1;
		char* input_province1;
		char* input_gpa;
		int int_id;
		int int_id1;
		float float_gpa;
		listData data;

		printf("명령어를 입력하세요: ");

		gets_s(buf, sizeof(buf));
		char* ptr = strtok(buf, " ");  //strtok	문자열 가르기 ""공백을 \n느낌으로?

		switch (ptr[0]) {
		case 'p':
			displayList(L);
			break;
		case 's':
			input_id1 = strtok(NULL, " ");
			int_id = atoi(input_id1);

			p = search(L, int_id);
			if (p == NULL)
				printf("찾는 데이터가 없습니다.\n");
			else {
				printf("%d 노드를 찾았습니다. 이름: %s \n", p->data.id, p->data.name);
			}
			break;
		case 'i':  //i <어디 값 뒤에 넣을거야?> <학번부터 말해줘> <이름> ....
			input_id1 = strtok(NULL, " ");
			int_id = atoi(input_id1);

			p = search(L, int_id);
			if (p == NULL)
				printf("찾는 데이터가 없습니다.\n");
			else {
				printf("%d 노드를 찾았습니다. 이름: %s \n", p->data.id, p->data.name);
				input_id2 = strtok(NULL, " ");
				int_id1 = atoi(input_id2);
				input_name1 = strtok(NULL, " ");
				input_phone1 = strtok(NULL, " ");
				input_province1 = strtok(NULL, " ");
				input_gpa = strtok(NULL, " ");
				float_gpa = atof(input_gpa);

				data.id = int_id1;
				strcpy(data.name, input_name1);
				strcpy(data.phone, input_phone1);
				strcpy(data.province, input_province1);
				data.gpa = float_gpa;

				insert(L, p, data);
			}
			displayList(L);
			break;
		case 'd':
			input_id1 = strtok(NULL, " ");
			int_id = atoi(input_id1);

			p = search(L, int_id);
			if (p == NULL)
				printf("찾는 데이터가 없습니다.\n");
			else {
				printf("%d 노드를 찾았습니다. 이름: %s \n", p->data.id, p->data.name);
				delete(L, p);
				printf("성공적으로 삭제하였습니다.\n");
			}
			
		case 'r':
			reverse(L);
			displayList(L);
			break;
		case 'g':
			printf("리스트에 저장된 데이터 개수: %d\n", L->length);
			break;
		case 'e':
			return 0;
			break;
		}

	}
}