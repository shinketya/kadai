#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define PHYSCHECK_NAME 1 /* 番号を表す定数値 */
#define PHYSCHECK_VISION 2 /* 氏名を表す定数値 */
#define PHYSCHECK_HEIGHT 4 /* 氏名を表す定数値 */
/*--- 会員データ ---*/
typedef struct {
  double vision;
  int height;
} Body;

typedef struct {
  Body body;
  char *name;
} PhysCheck;
/*--- ノード ---*/
typedef struct __node {
  PhysCheck data; /* データ */
  struct __node *next; /* 後続ポインタ*/
} Node;
/*--- 線形リスト ---*/
typedef struct {
  Node *head; /* 先頭ノードへのポインタ */
  Node *crnt; /* 着目ノードへのポインタ */
} List;
/*--- 会員の番号の比較関数 ---*/
int MemberHeightCmp(const PhysCheck *x, const PhysCheck *y){
  return x->body.height < y->body.height ? -1 : x->body.height > y->body.height ? 1 : 0;
}
int MemberVisionCmp(const PhysCheck *x, const PhysCheck *y){
  return x->body.vision < y->body.vision ? -1 : x->body.vision > y->body.vision ? 1 : 0;
}
/*--- 会員の氏名の比較関数 ---*/
int MemberNameCmp(const PhysCheck *x, const PhysCheck *y){
  return strcmp(x->name, y->name);
}
/*--- 会員データ(番号と氏名)の表示(改行なし)---*/
void PrintMember(const PhysCheck *x){
  printf("氏名 : %-18.18s 視力 : %.1lf 身長 : %d", x->name, x->body.vision, x->body.height);
}
/*--- 会員データ(番号と氏名)の表示(改行あり)---*/
void PrintLnMember(const PhysCheck *x){
  printf("氏名 : %-18.18s 視力 : %.1lf 身長 : %d\n", x->name, x->body.vision, x->body.height);
}
/*--- 会員データ(番号と氏名)の読込み ---*/
PhysCheck ScanMember(const char *message, int sw){
  PhysCheck temp;
  char name[80];
  printf("%s するデータを入力してください。\n", message);
  if (sw & PHYSCHECK_NAME) {
    printf("氏名:"); 
    scanf("%s", name);
    temp.name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(temp.name, name);
  }
  if (sw & PHYSCHECK_VISION) { printf("視力:"); scanf("%lf", &temp.body.vision); }
  if (sw & PHYSCHECK_HEIGHT) { printf("身長:"); scanf("%d", &temp.body.height); }
  return temp;
}
/*--- 一つのノードを動的に生成 ---*/
static Node *AllocNode(void){
  return calloc(1, sizeof(Node));
}
/*--- n の指すノードの各メンバーに値を設定 ----*/
static void SetNode(Node *n, const PhysCheck *x, const Node *next){
  n->data = *x; /* データ */
  n->next = next; /* 後続ポインタ */
}
/*--- 線形リストを初期化 ---*/
void Initialize(List *list){
  list->head = NULL; /* 先頭ノード */
  list->crnt = NULL; /* 着目ノード */
}
/*--- 関数 compare によって x と一致すると判定されるノードを探索 ---*/
Node *Search(List *list, const PhysCheck *x, Node *s, 
    int compare(const PhysCheck *x, const PhysCheck *y)){
  Node *ptr = s;
  while (ptr != NULL) {
    if (compare(&ptr->data, x) == 0) { /* キー値が一致 */
      list->crnt = ptr;
      return ptr; /* 探索成功 */
    }
    ptr = ptr->next; /* 後続ノードに着目 */
  }
  return NULL; /* 探索失敗 */
}
/*--- 先頭にノードを挿入 ---*/
void InsertFront(List *list, const PhysCheck *x){
  Node *ptr = list->head;
  list->head = list->crnt = AllocNode();
  SetNode(list->head, x, ptr);
}
/*--- 末尾にノードを挿入 ---*/
void InsertRear(List *list, const PhysCheck *x){
  if (list->head == NULL)
    /* 空であれば */
    InsertFront(list, x);
  /* 先頭に挿入 */
  else {
    Node *ptr = list->head;
    while (ptr->next != NULL)
      ptr = ptr->next;
    ptr->next = list->crnt = AllocNode();
    SetNode(ptr->next, x, NULL);
  }
}
/*--- 先頭ノードを削除 ---*/
void RemoveFront(List *list){
  if (list->head != NULL) {
    Node *ptr = list->head->next; /* 2番目のノードへのポインタ */
    free(list->head->data.name);
    free(list->head); /* 先頭ノードを解放 */
    list->head = list->crnt = ptr; /* 新しい先頭ノード */
  }
}
/*--- 末尾ノードを削除 ---*/
void RemoveRear(List *list){
  if (list->head != NULL) {
    if ((list->head)->next == NULL) /* ノードが一つだけであれば */
      RemoveFront(list);
    /* 先頭ノードを削除 */
    else {
      Node *ptr = list->head;
      Node *pre;
      while (ptr->next != NULL) {
        pre = ptr;
        ptr = ptr->next;
      }
      pre->next = NULL;
      free(ptr->data.name);
      free(ptr);
      list->crnt = pre;
      /* pre は末尾から 2 番目 */
      /* ptr は末尾 */
    }
  }
}
/*--- 着目ノードを削除 ---*/
void RemoveCurrent(List *list){
  if (list->head != NULL) {
    if (list->crnt == list->head) /* 先頭ノードに着目していれば */
      RemoveFront(list);
    /* 先頭ノードを削除 */
    else {
      Node *ptr = list->head;
      while (ptr->next != list->crnt)
        ptr = ptr->next;
      ptr->next = list->crnt->next;
      free(list->crnt);
      list->crnt = ptr;
    }
  }
}
/*--- 全ノードを削除 ---*/
void Clear(List *list){
  while (list->head != NULL)
    /* 空になるまで */
    RemoveFront(list);
  /* 先頭ノードを削除 */
  list->crnt = NULL;
}
/*--- 着目ノードのデータを表示 ---*/
void PrintCurrent(const List *list){
  if (list->crnt == NULL)
    printf("着目ノードはありません。");
  else
    PrintMember(&list->crnt->data);
}
/*--- 着目ノードのデータを表示(改行付き)---*/
void PrintLnCurrent(const List *list){
  PrintCurrent(list);
  putchar('\n');
}
/*--- 全ノードのデータをリスト順に表示 ---*/
void Print(const List *list){
  if (list->head == NULL)
    puts("ノードがありません。");
  else {
    Node *ptr = list->head;
    puts("【一覧表】");
    while (ptr != NULL) {
      PrintLnMember(&ptr->data);
      ptr = ptr->next; /* 後続ノードに着目 */
    }
  }
}
/*--- 線形リストの後始末 ---*/
void Terminate(List *list){
  Clear(list);
  /* 全ノードを削除 */
}
/*--- メニュー ---*/
typedef enum {
  TERMINATE, INS_FRONT, INS_REAR, RMV_FRONT, RMV_REAR, PRINT_CRNT,
  RMV_CRNT, SRCH_HEIGHT, SRCH_VISION, SRCH_NAME, PRINT_ALL, CLEAR
} Menu;
/*--- メニュー選択 ---*/
Menu SelectMenu(void){
  int i, ch;
  char *mstring[] = {
    "先頭にノードを挿入", "末尾にノードを挿入", "先頭のノードを削除",
    "末尾のノードを削除", "着目ノードを表示",
    "着目ノードを削除",
    "身長で探索", "視力で探索", "氏名で探索", "全ノードを表示", "全ノードを削除"
  };
  do {
    for (i = TERMINATE; i < CLEAR; i++) {
      printf("(%2d) %-18.18s ", i + 1, mstring[i]);
      if ((i % 3) == 2)
        putchar('\n');
    }
    printf("( 0) 終了 :");
    scanf("%d", &ch);
  } while (ch < TERMINATE || ch > CLEAR);
  return (Menu)ch;
}
/*--- メイン ---*/
int main(void){
  Menu menu;
  List list;
  PhysCheck x;
  Node *m = NULL;
  Initialize(&list);
  /* 線形リストの初期化 */
  do {
    switch (menu = SelectMenu()) {
      case INS_FRONT : /* 先頭にノードを挿入 */
        x = ScanMember("先頭に挿入", PHYSCHECK_NAME | PHYSCHECK_VISION |  PHYSCHECK_HEIGHT);
        InsertFront(&list, &x);
        break;
      case INS_REAR : /* 末尾にノードを挿入 */
        x = ScanMember("末尾に挿入", PHYSCHECK_NAME | PHYSCHECK_VISION |  PHYSCHECK_HEIGHT);
        InsertRear(&list, &x);
        break;
      case RMV_FRONT : /* 先頭ノードを削除 */
        RemoveFront(&list);
        break;
      case RMV_REAR : /* 末尾ノードを削除 */
        RemoveRear(&list);
        break;
      case PRINT_CRNT : /* 着目ノードのデータを表示 */
        PrintLnCurrent(&list);
        break;
      case RMV_CRNT : /* 着目ノードを削除 */
        RemoveCurrent(&list);
        break;

      case SRCH_HEIGHT : /* 番号による探索 */
        m = list.head;
        x = ScanMember("探索", PHYSCHECK_HEIGHT);
        if(Search(&list, &x, m, MemberHeightCmp) == NULL)
          puts("その身長のデータはありません。");
        while((m = Search(&list, &x, m, MemberHeightCmp)) != NULL){
          PrintLnCurrent(&list);
          m = m->next;
        }
        break;

      case SRCH_VISION : /* 番号による探索 */
        m = list.head;
        x = ScanMember("探索", PHYSCHECK_VISION);
        if(Search(&list, &x, m, MemberVisionCmp) == NULL)
          puts("その身長のデータはありません。");
        while((m = Search(&list, &x, m, MemberVisionCmp)) != NULL){
          PrintLnCurrent(&list);
          m = m->next;
        }
        break;

      case SRCH_NAME : /* 氏名による探索 */
        m = list.head;
        x = ScanMember("探索", PHYSCHECK_NAME);
        if(Search(&list, &x, m, MemberNameCmp) == NULL)
          puts("その身長のデータはありません。");
        while((m = Search(&list, &x, m, MemberNameCmp)) != NULL){
          PrintLnCurrent(&list);
          m = m->next;
        }
        break;

      case PRINT_ALL : /* 全ノードのデータを表示 */
        Print(&list);
        break;
      case CLEAR : /* 全ノードを削除 */
        Clear(&list);
        break;
    }
  } while (menu != TERMINATE);
  Terminate(&list);/* 線形リストの後始末 */
  return 0;
}
