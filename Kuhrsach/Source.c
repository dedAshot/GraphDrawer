#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>


typedef struct node
{
	char el[4];
	double value;
	struct node* left;
	struct node* right;
	struct node* prev;
} TREENODE;

TREENODE* HeadTree = NULL;
//changed complite
TREENODE* insertNode(TREENODE* tree, char* u)
{
	TREENODE* newNode = (TREENODE*)malloc(sizeof(TREENODE));
	TREENODE* root = tree;
	TREENODE* q; 

	if (newNode != NULL)
	{
		newNode->el[0] = NULL;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->prev = NULL;
	}
	else
	{
		puts("Error!");
		return NULL;
	}

	//printf("n\n");

	if (!IsOperation(u))
	{ 
		//printf("%c |", *u);
		if (*u != 'x')
		{
			newNode->value = strtod(u, NULL); //перевести char в double
			//printf("%lf | \n", newNode->value);
		}
		else
		{
			newNode->el[0] = 'x';
			//printf("%c | \n", newNode->el);
		}
		if (root == NULL) return newNode;/*пустое дерево*/

		newNode->prev = root;
		if (root->left != NULL)
			root->right = newNode;/* ( правый узел оператора) */
		else
			root->left = newNode;
		return root;
	}
	else
	{
		//printf(" %c |", *u);
		newNode->el[0] = *u;
		if ((*(u) == 's' && *(u + 1) == 'i' && *(u + 2) == 'n') || (*(u) == 'c' && *(u + 1) == 'o' && *(u + 2) == 's') || (*(u) == 't' && *(u + 1) == 'g') || (*(u) == 'c' && *(u + 1) == 't' && *(u + 2) == 'g'))
		{
			newNode->el[1] = *(u+1);
			if (*(u+1) == 'o' || *(u+1) == 'i' || *(u+1) == 't') newNode->el[2] = *(u+2);
			
			//Head->left = createNodes(tree, ++i, str, len);
			//q = Head;
			//printf("| trigonom: %c |\n", *(u));
			//skipIndex(&i, str);
		}

		if (prior(u) > prior(&root->el))
		{
			newNode->left = root->right;
			root->right = newNode;
			newNode->prev = root;
			if (newNode->left != NULL)
				newNode->left->prev = newNode;
			return newNode;
		}
		else
		{
			while ((prior(u) < prior(&root->el)) && (root->prev != NULL)) root = root->prev;
			//printf("BLADBLAD %c", *u);
			newNode->left = root;
			if (root->prev != NULL)
			{
				root->prev->right = newNode;
				newNode->prev = root->prev;
			}
			root->prev = newNode;
			return newNode;
		}
	}
}

void deleteTree(TREENODE* tree)
{
	TREENODE* q = tree;/*указатель на текущий узел*/

	if (q->left != NULL) return deleteTree(q->left);
	if (q->right != NULL) return deleteTree(q->right);
	free(q);
}
//changed complite
int IsOperation(char *ch)
{
	if (*ch == '+' || *ch == '-' || *ch == '*' || *ch == '/' || *ch == '^' || *ch == '(' || *ch == ')' || *ch == 's' || *ch == 'c' || *ch == 't')
		return 1;

	return 0;
}
//changed complite
double calc(TREENODE* Head, double x, int *err)
{
	//printf("c\n");
	if ((Head->el[0] != NULL) && (Head->el[0] != 'x'))
	{
		switch (Head->el[0])
		{
		case '+': {
			return (double)calc(Head->left, x, err) + calc(Head->right, x, err);
		}
		case '-': {
			return (double)calc(Head->left, x, err) - calc(Head->right, x, err);
		}
		case '*': {
			return (double)calc(Head->left, x, err) * calc(Head->right, x, err);
		}
		case '/': {
			if (((double)calc(Head->right, x, err)) != 0.0000000000000000)
				return (double)calc(Head->left, x, err) / calc(Head->right, x, err);
			else
			{
				//printf("ERR: /0, value=%lf", (double)calc(Head->right, x, err));
				*err = 1;
				break;
			}
		}
		case '^': {
			if (!((double)calc(Head->left, x, err) == 0.0000000000000000 && (double)calc(Head->right, x, err) == 0.0))
			{
				if (((double)calc(Head->left, x, err) < 0.0) && ((double)calc(Head->right, x, err) != (int)calc(Head->right, x, err)))
				{
					//puts("ERR: -value^(double)");
					*err = 1;
					break;
				}
				else
					return (double)pow(calc(Head->left, x, err), calc(Head->right, x, err));
			}
			else
			{
				//puts("ERR: 0^0");
				*err = 1;
				break;
			}
		}
		case 's': {
				return sin((double)calc(Head->left, x, err));
		}
		case 'c': {
			if (Head->el[1]=='o') return cos((double)calc(Head->left, x, err));
			else {
				if (tan((double)calc(Head->left, x, err)) != 0.0000000000000000) return (double)1.0 / tan((double)calc(Head->left, x, err));
				else {
					*err = 1;
					break;
				}
			}
		}
		case 't': {
			return (double)tan((double)calc(Head->left, x, err));
		}
		}
	}
	else
	{
		if (Head->el[0] == 'x')
			return (double)x;
		else
		{
			//printf(" %lf || ", Head->value);
			return (double)Head->value;
		}
	}
}
//changed (char* oper) complite
int prior(char* oper)
{
	if (*oper == '+' || *oper == '-')
		return 1;
	else if (*oper == '*' || *oper == '/')
		return 2;
	else if (*oper == '^')
		return 3;
	else if (*oper == '(' || *oper == ')' || *oper == 's' || *oper == 'c' || *oper == 't')
		return 4;
	return 10;
}

void med(TREENODE* tree)
{
	if (tree != NULL)
	{
		med(tree->left);
		if (tree->el[0] == NULL)
			printf("%lf |", tree->value);
		else
			printf("%c |", tree->el[0]);
		med(tree->right);
	}
}

//1.000000 |/ |x |+ |x |^ |2.000000 |+ |4.000000 |* |x |

//cos(x+0)+sin(x+0)

//1.0+2.0*3.0-4.0
//4.0*7.0+2.0-3.0/4.0
//2.0*(3.0+4.0)
//2.0*(3.0+4.0)
//(3.0+4.0)*(5.0+6.0)
//(((1.0+2.0)*3.0)*4.0)+2
//((3.0+4.0)*(5.0+6.0))+2

void skipIndex(int* index, char* str)
{
	int count = 1;
	int i = *index;
	while (count)
	{
		if (*(str + i) == '(') count++;
		else if (*(str + i) == ')') count--;
		i++;
	}
	*index = i;
}
//changed
TREENODE* createNodes(TREENODE* tree, int startIndex, char* str, int len) //perem tree should delete
{
	TREENODE* q, * Head;
	int i = startIndex, count=0;

	q = (TREENODE*)malloc(sizeof(TREENODE));
	if (q != NULL)
	{
		q->el[0] = NULL;
		q->left = NULL;
		q->right = NULL;
		q->prev = NULL;
	}
	else
	{
		puts("Error!");
		return NULL;
	}

	Head = (TREENODE*)malloc(sizeof(TREENODE));
	if (Head != NULL)
	{
		Head->el[0] = NULL;
		Head->left = NULL;
		Head->right = NULL;
		Head->prev = NULL;

		q->prev = Head;
	}
	else
	{
		puts("Error!");
		return NULL;
	}

	if (*(str + i) == '(') //if the first symbol is ( or trigonom func
	{
		Head->left = createNodes(tree, ++i, str, len);
		skipIndex(&i, str);
		Head->el[0] = *(str + i++);
		q = Head;

		if (*(str + i) == '(')
		{
			q->right = createNodes(tree, (++i), str, len);
			skipIndex(&i, str);
		}
		if (*(str + i) == ')')
			return Head;
	}
	else if ((*(str + i) == 's' && *(str + i+1) == 'i' && *(str + i+2) == 'n') || (*(str + i) == 'c' && *(str + i + 1) == 'o' && *(str + i + 2) == 's') || (*(str + i) == 't' && *(str + i + 1) == 'g') || (*(str + i) == 'c' && *(str + i + 1) == 't' && *(str + i + 2) == 'g'))
	{
		Head->el[0] = *(str + i++);
		Head->el[1] = *(str + i);
		if (*(str + i) == 'o' || *(str + i) == 'i' || *(str + i) == 't') Head->el[2] = *(str + ++i);
		i++;
		Head->left=createNodes(tree, ++i, str, len);
		q = Head;
		//printf("| %c |\n", *(str + i));
		skipIndex(&i, str);
		//printf("| %d |\n", i);
	}
	else
	{
		if (*(str + i) != 'x')
			q->value = strtod((str + i), NULL);
		else
			q->el[0] = 'x';

		while (!IsOperation(++i + str));

		Head->el[0] = *(str + i);
		Head->left = q;	
		i++;
		q = Head;

		if (*(str + i) == '(')
		{
			q->right = createNodes(tree, (++i), str, len);//<--
			skipIndex(&i, str);
		}
	}

	//printf("s\n");
	while (i < len)//suda dobavit trigonom func
	{
		q = insertNode(q, str + i);

		if (IsOperation(str + i))
		{
			if ((*(str + i) == 's' && *(str + i + 1) == 'i' && *(str + i + 2) == 'n') || (*(str + i) == 'c' && *(str + i + 1) == 'o' && *(str + i + 2) == 's') || (*(str + i) == 't' && *(str + i + 1) == 'g') || (*(str + i) == 'c' && *(str + i + 1) == 't' && *(str + i + 2) == 'g'))
			{
				if (*(str + i+1) == 'o' || *(str + i+1) == 'i' || *(str + i+1) == 't') i++;
				i+=2;
				q->left = createNodes(tree, ++i, str, len);
				//printf("| %c |\n", *(str + i));
				skipIndex(&i, str);
				//printf("| %d char: %c |\n", i, *(str + i));
			}
			//med(q);
			i++;
		}
		else 
			while ((++i < len) && (!IsOperation(str + i)));

		if (*(str + i) == '(')
		{
			q->right=createNodes(tree, (++i), str, len);
			skipIndex(&i, str);
		}
		if (*(str + i) == ')')
		{
			while (Head->prev != NULL) Head = Head->prev;//<--
			return Head;
		}
					
	}
	while (Head->prev != NULL) Head = Head->prev;

	return Head;
}

//LRESULT WndProcu(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
//{
//	if (message == WM_DESTROY || wparam==27)
//		PostQuitMessage(0);
//	else if (message == WM_KEYDOWN)
//	{
//		printf("key pressed/n");
//	}
//	else return DefWindowProcA(hwnd, message, wparam, lparam);
//}

void firstmsg()
{
	system("cls");
	printf("\n\n\n");
	printf("-----------------------------------------------ПОСТРОЕНИЕ ГРАФИКОВ ФУНКЦИЙ----------------------------------------------\n\n");
	printf("\t\t\t\tВ данной программе вы можете простроить график функции от х в заданном диапазоне.\n\t\t\t\tУправление осуществляется при помощи клавиш курсора, клвиши \"Enter\" и \"ESC\".\n\t\t\t\tЧтобы продолжить нажмити \"Enter\"\n\n");
	printf("--------------------------------------------------------Об авторе-------------------------------------------------------\n\n");
	printf("\t\t\t\t\t\t\tБрГТУ, ФЭИС\n");
	printf("\t\t\t\t\t\t\tCтудент 2 курса\n");
	printf("\t\t\t\t\t\t\tГруппы МС-6\n");
	printf("\t\t\t\t\t\t\tКозел Н.В.\n");
}

void showMenu(int index)
{
	int i = 0;
	char list[][20] = { "Построить график","Справка","Об авторе", "Выход"};

	system("cls");
	printf("\n\n\n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	for (i=0; i<4;i++)
	{
		
		printf("\t\t\t\t\t\t");
		if (i == index) printf(">%s<\n", list[i]);
		else printf("%s\n", list[i]);
	}
	printf("------------------------------------------------------------------------------------------------------------------------\n");

}
//changed complite
void graph()
{
	TREENODE* q;
	char str[100];
	int i = 0, j = 0, len, counter = 0;
	const wh = 1000;
	const wl = 1000;
	double xmax, x, dx, dxt, y, yt, t, lenx1, lenx2, leny1, leny2, nx, ny, cx, cy;
	int p;
	int err = 0;

	HWND hwnd = GetConsoleWindow();

	HDC dc = GetDC(hwnd);

	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, RGB(255, 255, 255));

	system("cls");
	printf("Введите промежутки\n");
	printf("x:");
	scanf("%lf %lf", &lenx1, &lenx2);
	printf("y:");
	scanf("%lf %lf", &leny1, &leny2);

	nx = wl / (lenx2 - lenx1);
	ny = wh / (leny2 - leny1);


	printf("Введите функцию:\n");
	scanf("%s", str);

	len = strlen(str);
	HeadTree = createNodes(HeadTree, 0, str, len);

	//med(HeadTree);

	system("cls");

	Rectangle(dc, 0, 0, 1000, 1000);
	dx = 1/nx;
	x = lenx1;
	xmax = lenx2;

	cx = nx * fabs(lenx1);
	cy = ny * fabs(leny2);

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	MoveToEx(dc, 0, cy, NULL);
	LineTo(dc, wl, cy);

	MoveToEx(dc, cx, 0, NULL);
	LineTo(dc, cx, wh);

	do
	{
		err = 0;
		yt = calc(HeadTree, x, &err);
		x += dx;
		
	} while (err && (x < xmax));


	while (x < xmax)
	{
		if (!err)
		{
			y = calc(HeadTree, x, &err);
			t = (double)fabs((y - yt)*ny);
		}
		else
		{
			err = 0;
			y = calc(HeadTree, x, &err);
			t = 1;
		}
		dxt = dx;
		//t = 1;
		while ((double)t < 0.4 && dx<1/nx && !err && (yt < leny2) && (yt > leny1))// iskluchenie priamoi linii
		{
			x -= dx;
			dx *= 2;
			x += dx;
			y = (calc(HeadTree, x, &err));

			if (err)
			{
				dx = dxt;
				break;
			}
			t = (double)fabs((y - yt) * ny);
			//printf("ASDFASDF t: %lf dx: %lf y: %lf ty: %lf absT: %lf ", t, dx, y, yt, (double)fabs((y - yt) * ny));
		}
		while ((double)t > 1.0 && !err && (yt < leny2) && (yt > leny1))
		{
			x -= dx;
			//if (t > 1) dx /= 2;
			dx /= 2.0;
			x += dx;
			y = (calc(HeadTree, x, &err));

			if (err)
			{
				dx = dxt;
				break;
			}
			t = (double)fabs((y - yt)*ny);
			//printf(" t: %lf dx: %lf y: %lf ty: %lf absT: %lf ", t ,dx, y ,yt, (double)fabs((y - yt) * ny));
			
		}

		

		if (!err)
		{
			//LineTo(dc, cx + x*nx, cy - y*ny);
			SetPixel(dc, cx + x * nx, (cy - y * ny), RGB(0,0,0));
			yt = y;
		}

		x += dx;
	}
	//med(HeadTree);
	//while (getch() != 13);
}

void show1()
{
	system("cls");
	printf("\n\n\n");
	printf("--------------------------------------------------------Справка---------------------------------------------------------\n");
	printf("\t\t\t\tДля начала работы программы выберите пункт \"Построить график\", \n\t\t\t\tдалее введите интересующий промежуток(через пробел) по оси x, затем по оси y, \n\t\t\t\tдалее введите функцию(без пробелов, не менее 2 операндов). \n\n\t\t\t\tЧтобы вернуться в меню нажмите ESC, \n\t\t\t\tесли хотите посмотреть другой график нажмите любую другую клавишу.\n");
	printf("\t\t\t\tДля выхода из пунктов меню нажмите ESC\n");
	printf("\t\t\t\tДля завершения работы программы нажмите ESC в меню, \n");
	printf("\t\t\t\tлибо подпункт меню \"Выход\"\n");
}

void show2()
{
	system("cls");
	printf("\n\n\n");
	printf("--------------------------------------------------------Об авторе-------------------------------------------------------\n");
	printf("\t\t\t\t\t\tБрГТУ, ФЭИС\n");
	printf("\t\t\t\t\t\tCтудент 2 курса\n");
	printf("\t\t\t\t\t\tГруппы МС-6\n");
	printf("\t\t\t\t\t\tКозел Н.В.\n");


}

int main()
{
	const wh = 1000;
	const wl = 1000;

	system("chcp 1251");

	HWND hwnd = GetConsoleWindow();
	RECT r;
	GetClientRect(hwnd, &r);
	MoveWindow(hwnd, 100, 100, wl, wh, TRUE);

	HDC dc = GetDC(hwnd);

	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, RGB(255, 255, 255));
	
	/*WNDCLASSA wcl;
	memset(&wcl, 0, sizeof(WNDCLASSA));
	wcl.lpszClassName = "my Window";
	wcl.lpfnWndProc = DefWindowProcA;
	RegisterClassA(&wcl);

	HWND hwnd;
	hwnd = CreateWindow("my Window", "Window Name", WS_OVERLAPPEDWINDOW, 10, 10, 1000, 1000, NULL, NULL, NULL, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg;
	while (1)
	{
		DispatchMessage(&msg);
	}*/

	system("cls");

	int k = 0;
	int index = 0;

	firstmsg();
	while (getch() != 13) firstmsg();

	do
	{
		switch (k)
		{
		case 72: if (index > 0) index--;
			break;

		case 80: if (index < 3) index++;
			break;

		case 13:
			switch (index)
			{
			case 0:
				graph();
				while (getch() != 27)graph();
				break;
			case 1:
				show1();
				while (getch()!=27) show1();
				break;
			case 2:
				show2();
				while (getch() != 27) show2();
				break;
			case 3:
				return 0;
				break;
			}
			break;
		}
		showMenu(index);

	} while ((k = getch()) != 27);

	return 0;
}

//gets(str);
//len = strlen(str);
//HeadTree = createNodes(HeadTree, 0, str, len);
//
//med(HeadTree);
//system("cls");
//Rectangle(dc, 0, 0, 1000, 1000);
//dx = 1.0;
//x = -498.0;
//xmax = 498.0;
//
//HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
//
//MoveToEx(dc, 0, cy, NULL);
//LineTo(dc, wl, cy);
//
//MoveToEx(dc, cx, 0, NULL);
//LineTo(dc, cx, wh);
//
//do
//{
//	yt = calc(HeadTree, x, &err);
//	MoveToEx(dc, x + cx, cy - yt, NULL);
//	x += dx;
//} while (err && (x < xmax));
//
//while (x < xmax)
//{
//	counter++;
//	if (!err)
//	{
//		y = calc(HeadTree, x, &err);
//		t = abs((int)y - (int)yt);
//	}
//	else
//	{
//		err = 0;
//		y = calc(HeadTree, x, &err);
//		t = 1;
//	}
//	dxt = dx;
//	t = 1;
//	while (t > 2.0 && !err)
//	{
//		x -= dx;
//		if (t > 1) dx /= 2;
//		x += dx;
//		y = (calc(HeadTree, x, &err));
//
//		if (err)
//		{
//			dx = dxt;
//			break;
//		}
//		t = abs((int)y - (int)yt);
//	}
//
//	if (!err)
//	{
//		LineTo(dc, cx + x, cy - y);
//		yt = y;
//	}
//
//	x += dx;
//}