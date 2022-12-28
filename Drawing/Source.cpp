#include "graphics.h"
#include <iostream>
#include <math.h>
#pragma comment(lib,"graphics.lib")
#pragma warning(disable : 4996)
using namespace std;
struct Node {
	int coeff;
	int powx;
	int powy;
	int powz;
	struct Node* next;
};
void create_node(int c, int p1, int p2, int p3, struct Node** temp)
{
	Node* r = (Node*)malloc(sizeof(Node)); // Stores new node
	Node* z = *temp; // Stores temp node
	r->coeff = c; // Update coefficient of r
	r->powx = p1; // Update power of variable x in r
	r->powy = p2; // Update power of variable y in r
	r->powz = p3; // Update power of variable z in r
	if (z == nullptr) {
		(*temp) = r; // Update temp node
		(*temp)->next = (*temp); // Update next pointer of temp node
	}
	else {
		r->next = z->next; // Update next pointer of z
		z->next = r; // Update next pointer of z
		*temp = r; // Update temp Node
	}
}
void draw_Node(int left, int top, int right, int bottom, Node* node) {
	if (node->coeff == 0) return;
	char* numberstring = (char*)malloc(sizeof(char));
	sprintf(numberstring, "%d", node->coeff);
	outtextxy(left + 42, top + 2, numberstring);
	sprintf(numberstring, "%d", node->powx);
	outtextxy(left + 12, bottom + 7, numberstring);
	sprintf(numberstring, "%d", node->powy);
	outtextxy(left + 42, bottom + 7, numberstring);
	sprintf(numberstring, "%d", node->powz);
	outtextxy(left + 72, bottom + 7, numberstring);
	rectangle(left     , top     , right, bottom);
	rectangle(left     , bottom, right, bottom + 30);
	rectangle(left + 30, bottom, right, bottom + 30);
	rectangle(left + 60, bottom, right, bottom + 30);
}
void draw_Poly(int left, int top, int right, int bottom, Node* node) {
	int l = left;
	int r = right;
	Node* temp = node;
	node = node->next;
	while (node != temp) {
		if (node->coeff != 0) {
			draw_Node(l, top, r, bottom, node);
			l += 100;
			r += 100;
			if (r >= getmaxwidth()) {
				l = left;
				r = right;
				top += 70;
				bottom += 70;
			}
		}
		node = node->next;
		delay(100);
	}
	if (node->coeff != 0) draw_Node(l, top, r, bottom, node);
}
// Display the circular linked list
void display(struct Node* node)
{
	Node* start = node; // Stores head node of list
	Node* temp = (Node*)malloc(sizeof(Node));
	node = node->next; // Update node
	while (node != start && node->coeff != 0) {

		cout << node->coeff; // Print coefficient of current node	

		if (node->powx != 0) cout << "x^" << node->powx;
		if (node->powx != 0 && node->powy != 0) cout << "*";
		if (node->powy != 0) cout << "y^" << node->powy;
		if ((node->powy != 0 && node->powz != 0) || (node->powx != 0 && node->powz != 0)) cout << "*";
		if (node->powz != 0) cout << "z^" << node->powz;

		if (node != start && node->next->coeff != 0) // Add next term of the polynomial
			cout << " + ";

		node = node->next; // Update node
	}
	if (node->coeff != 0) {
		cout << node->coeff;
		if (node->powx != 0) cout << "x^" << node->powx;
		if (node->powx != 0 && node->powy != 0) cout << "*";
		if (node->powy != 0) cout << "y^" << node->powy;
		if ((node->powy != 0 && node->powz != 0) || (node->powx != 0 && node->powz != 0)) cout << "*";
		if (node->powz != 0) cout << "z^" << node->powz;
	}
	cout << "\n\n";
}
int checkABC(Node* N1, Node* N2) {
	if (N1->powx == N2->powx && N1->powy == N2->powy && N1->powz == N2->powz) return 0;
	if ((N1->powx < N2->powx)
		|| (N1->powx == N2->powx && N1->powy < N2->powy)
		|| (N1->powx == N2->powx && N1->powy == N2->powy && N1->powz < N2->powz)) return -1;
	return 1;
}
// Add polynomials and draw on the graphics window
void add_polynomials(Node* P, Node* Q) {
	initwindow(getmaxwidth(), getmaxheight());
	draw_Poly(10, 10, 100, 30, P);
	delay(500);
	draw_Poly(10, 80, 100, 100, Q);
	delay(500);

	int x1 = 10, y1 = 10;
	int x2 = 10, y2 = 80;
	int xRes = 10;

	create_node(0, 0, 0, -1, &P);
	create_node(0, 0, 0, -1, &Q);
	P = P->next;
	Node* Q1 = Q;
	Q = Q->next;
	Node* Q2 = (Node*)malloc(sizeof(Node));
	Node* Res = nullptr;
	setcolor(LIGHTGREEN);
	setfillstyle(0, 0);
	while (true) {
		int check = checkABC(P, Q);
		if (check == -1) {
			Q1 = Q;
			create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
			Q = Q->next;
			char* p;
			int n = imagesize(0, 0, 100, 60);
			p = (char*)malloc(n);
			getimage(x2, y2, x2 + 90, y2 + 51, p);
			float x = x2, y = y2, tempX = 0, tempY = 0;
			float addX = abs(x - xRes) / 100, addY = abs(y - 150) / 100;
			bar(x2, y2, x2 + 91, y2 + 51);
			do {
				putimage(x + tempX, y + tempY, p, 1);
				delay(5);
				putimage(x + tempX, y + tempY, p, 1);
				tempX += addX;
				tempY += addY;
			} while (y + tempY < 150);
			putimage(x + tempX, y + tempY, p, 1);
			delete(p);
			x2 += 100;
			xRes += 100;
			delay(5);
		}
		else if (check == 0) {
			if (P->powz < 0) {
				char* p;
				int n = imagesize(0, 0, 1200, 60);
				p = (char*)malloc(n);
				getimage(10, 150, xRes, 210, p);
				bar(10, 150, xRes, 210);
				float temp = 0;
				do {
					putimage(10, 150 - temp, p, 1);
					delay(5);
					putimage(10, 150 - temp, p, 1);
					temp += 1.4;
				} while (150 - temp > 10);
				putimage(10, 150 - temp, p, 1);
				delete(p);
				display(Res);
				getch();
				closegraph();
				return;
			}
			Q->coeff += P->coeff;
			if (Q->coeff == 0) {
				Q2 = Q;
				Q1->next = Q = Q->next;
				create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
				P = P->next;
			}
			else {
				P = P->next;
				Q1 = Q;
				create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
				Q = Q->next;
				char* p;
				int n = imagesize(0, 0, 100, 60);
				p = (char*)malloc(n);
				getimage(x1, y1, x1 + 90, y1 + 50, p);
				float x = x1, y = y1, tempX = 0, tempY = 0;
				float addX = (x - x2) / 100, addY = abs(y - y2) / 100;
				bar(x1, y1, x1 + 91, y1 + 51);
				do {
					putimage(x + tempX, y + tempY, p, 1);
					delay(5);
					putimage(x + tempX, y + tempY, p, 1);
					if (x1 > x2) tempX -= addX;
					else tempX += addX;
					tempY += addY;
				} while (y + tempY < y2);
				putimage(x + tempX, y + tempY, p, 1);
				bar(x2, y2, x2 + 91, y2 + 51);
				draw_Node(x2, y2, x2 + 90, y2 + 20, Q1);
				getimage(x2, y2, x2 + 90, y2 + 51, p);
				x = x2, y = y2, tempX = 0, tempY = 0;
				addX = abs(x - xRes) / 100, addY = abs(y - 150) / 100;
				bar(x2, y2, x2 + 91, y2 + 51);
				do {
					putimage(x + tempX, y + tempY, p, 1);
					delay(5);
					putimage(x + tempX, y + tempY, p, 1);
					tempX += addX;
					tempY += addY;
				} while (y + tempY < 150);
				putimage(x + tempX, y + tempY, p, 1);
				delete(p);
				x1 += 100;
				x2 += 100;
				xRes += 100;
				delay(5);
			}
		}
		else {
			Q2->coeff = P->coeff;
			Q2->powx = P->powx;
			Q2->powy = P->powy;
			Q2->powz = P->powz;
			Q2->next = Q;
			Q1->next = Q2;
			Q1 = Q2;
			create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
			P = P->next;
			char* p;
			int n = imagesize(0, 0, 100, 60);
			p = (char*)malloc(n);
			getimage(x1, y1, x1 + 90, y1 + 50, p);
			float x = x1, y = y1, tempX = 0, tempY = 0;
			float addX = abs(x - xRes) / 100, addY = abs(y - 150) / 100;
			bar(x1, y1, x1 + 91, y1 + 51);
			do {
				putimage(x + tempX, y + tempY, p, 1);
				delay(5);
				putimage(x + tempX, y + tempY, p, 1);
				tempX += addX;
				tempY += addY;
			} while (y + tempY < 150);
			putimage(x + tempX, y + tempY, p, 1);
			delete(p);
			x1 += 100;
			xRes += 100;
			delay(5);
		}
	}
}
// Add polynomials without draw
Node* add_nondraw(Node* P, Node* Q) {
	create_node(0, 0, 0, -1, &P);
	create_node(0, 0, 0, -1, &Q);
	P = P->next;
	Node* Q1 = Q;
	Q = Q->next;
	Node* Q2 = (Node*)malloc(sizeof(Node));
	Node* Res = nullptr;
	while (true) {
		int check = checkABC(P, Q);
		if (check == -1) {
			Q1 = Q;
			create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);

			Q = Q->next;
		}
		else if (check == 0) {
			if (P->powz < 0) return Res;
			Q->coeff += P->coeff;
			if (Q->coeff == 0) {
				Q2 = Q;
				Q1->next = Q = Q->next;
				create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
				P = P->next;
			}
			else {
				P = P->next;
				Q1 = Q;
				create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
				Q = Q->next;
			}
		}
		else {
			Q2->coeff = P->coeff;
			Q2->powx = P->powx;
			Q2->powy = P->powy;
			Q2->powz = P->powz;
			Q2->next = Q;
			Q1->next = Q2;
			Q1 = Q2;
			create_node(Q1->coeff, Q1->powx, Q1->powy, Q1->powz, &Res);
			P = P->next;
		}
	}
	return Res;
}
// Multiple polynomials and draw on the graphics window
void mul_polynomials(Node* P, Node* Q) {
	initwindow(getmaxwidth(), getmaxheight());
	draw_Poly(10, 10, 100, 30, P);
	delay(500);
	draw_Poly(10, 80, 100, 100, Q);
	delay(500);
	setcolor(MAGENTA);
	for (int i = 5; i <= 1405; i += 100) {
		line(i, 140, i+100, 140);
		delay(100);
	}
	for (int i = 5; i <= 1405; i += 100) {
		line(i, 420, i + 100, 420);
		delay(100);
	}

	int x2 = 10, y2 = 80;
	int yMul = 150, yRes = 430, yAdd = 290;

	create_node(0, 0, 0, -1, &P);
	create_node(0, 0, 0, -1, &Q);
	bool check = true;
	Node* Res = nullptr;
	create_node(0, 0, 0, -1, &Res);
	Node* temp = nullptr;
	Node* mulNode = nullptr;
	setfillstyle(0, 0);
	do {
		Q = Q->next;
		if (Q->powz < 0) break;
		setcolor(YELLOW);
		draw_Node(x2, y2, x2 + 90, y2 + 20, Q);
		int x1 = 10, y1 = 10;
		int xMul = 10;
		int xRes = 10;
		P = P->next;
		while (P->powz > 0) {
			create_node(P->coeff * Q->coeff, P->powx + Q->powx, P->powy + Q->powy, P->powz + Q->powz, &mulNode);
			create_node(P->coeff * Q->coeff, P->powx + Q->powx, P->powy + Q->powy, P->powz + Q->powz, &temp);
			char* p;
			int n = imagesize(0, 0, 100, 60);
			p = (char*)malloc(n);
			getimage(x2, y2, x2 + 90, y2 + 50, p);
			float x = x2, y = y2, tempX = 0, tempY = 0;
			float addX = abs(x - x1) / 100, addY = abs(y - y1) / 100;
			//bar(x1, y1, x1 + 91, y1 + 51);
			do {
				putimage(x + tempX, y - tempY, p, 1);
				delay(5);
				putimage(x + tempX, y - tempY, p, 1);
				if (x2 > x1) tempX -= addX;
				else tempX += addX;
				tempY += addY;
			} while (y - tempY > y1);
			char* p2;
			p2 = (char*)malloc(n);
			getimage(x1, y1, x1 + 90, y1 + 50, p2);
			bar(x1, y1, x1 + 91, y1 + 51);
			setcolor(LIGHTRED);
			draw_Node(x1, y1, x1 + 90, y1 + 20, mulNode);
			getimage(x1, y1, x1 + 90, y1 + 50, p);
			bar(x1, y1, x1 + 91, y1 + 51);
			putimage(x1, y1, p2, 1);
			delete(p2);
			x = x1, y = y1, tempX = 0, tempY = 0;
			addX = abs(x - xMul) / 100, addY = abs(y - yMul) / 100;
			do {
				putimage(x + tempX, y + tempY, p, 1);
				delay(5);
				putimage(x + tempX, y + tempY, p, 1);
				tempX += addX;
				tempY += addY;
			} while (y + tempY < yMul);
			putimage(x + tempX, y + tempY, p, 1);
			delete(p);
			x1 += 100;
			xMul += 100;
			delay(5);
			mulNode = nullptr;
			P = P->next;
		}
		delay(1000);
		bar(x2, y2, x2 + 91, y2 + 51);
		Res = add_nondraw(Res, temp);
		setcolor(LIGHTGREEN);
		draw_Poly(10, yRes, 100, yRes + 20, Res);
		delay(1000);
		bar(10, yAdd, getmaxwidth(), yAdd + 130);
		char* p;
		int n = imagesize(0, 0, getmaxwidth(), 150);
		p = (char*)malloc(n);
		getimage(10, yRes - 1, getmaxwidth(), yRes + 140, p);
		float y = yRes, tempY = 0;
		float addY = abs(y - yAdd) / 100;
		delay(1000);
		bar(10, yRes, getmaxwidth(), yRes + 140);
		if (Q->next->powz > 0) {
			do {
				putimage(10, y - tempY, p, 1);
				delay(5);
				putimage(10, y - tempY, p, 1);
				tempY += addY;
			} while (y - tempY > yAdd);
			putimage(xRes, y - tempY, p, 1);
		}
		delete(p);
		delay(500);
		bar(10, yMul, getmaxwidth(), yMul + 140);
		temp = nullptr;
		x2 += 100;
		delay(1000);
	} while (true);
	clearviewport();
	draw_Poly(10, 10, 100, 30, Res);
	display(Res);
	getch();
}
// Enter polynomials
void input(Node* &Poly, int n) {
	int c1, powx, powy, powz;
	for (int i = 0; i < n; i++) {
		cout << "Nhap he so: ";
		cin >> c1;
		cout << "Nhap so mu cua x: ";
		cin >> powx;
		cout << "Nhap so mu cua y: ";
		cin >> powy;
		cout << "Nhap so mu cua z: ";
		cin >> powz;
		create_node(c1, powx, powy, powz, &Poly);
	}
}
void init(Node* &Poly1, Node* &Poly2) {
	int n1, n2;
	cout << "Nhap so hang tu cua da thuc thu nhat: ";
	cin >> n1;
	input(Poly1, n1);
	cout << "Nhap so hang tu cua da thuc thu hai: ";
	cin >> n2;
	input(Poly2, n2);
}
void option() {
	// Stores node of first polynomial
	Node* poly1 = nullptr;

	// Stores node of second polynomial
	Node* poly2 = nullptr;
	int n = 0;

	while (n != 3) {
		// Option
		cout << "1. Su dung hai da thuc duoc tao san.\n2. Nhap da thuc moi.\n3. Ket thuc chuong trinh.\n";
		cin >> n;
		while (n != 1 && n != 2 && n != 3) {
			cout << "Vui long nhap lai lua chon!\n";
			cout << "1. Su dung hai da thuc duoc tao san.\n2. Nhap da thuc moi.\n3. Ket thuc chuong trinh.\n";
			cin >> n;
		}
		if (n == 1) {
			// Create first polynomial
			create_node(4, 3, 2, 2, &poly1);
			create_node(5, 2, 1, 4, &poly1);
			create_node(3, 1, 1, 3, &poly1);
			create_node(2, 1, 1, 2, &poly1);
			create_node(2, 1, 0, 4, &poly1);
			create_node(3, 0, 1, 3, &poly1);
			create_node(2, 0, 0, 1, &poly1);

			// Create second polynomial
			create_node(3, 2, 1, 4, &poly2);
			create_node(4, 1, 0, 4, &poly2);
			create_node(2, 0, 1, 3, &poly2);
			create_node(6, 0, 0, 2, &poly2);
			create_node(6, 0, 0, 1, &poly2);
		}
		else if (n == 2) init(poly1, poly2);
		else return;

		// Display polynomial 1
		cout << "Da thuc thu nhat" << "\n";
		display(poly1);

		// Display polynomial 2
		cout << "Da thuc thu hai" << "\n";
		display(poly2);

		cout << "\n1. Cong hai da thuc.\n2. Nhan hai da thuc.\n";
		cin >> n;
		while (n != 1 && n != 2) {
			cout << "Vui long nhap lai lua chon!\n";
			cout << "1. Cong hai da thuc.\n2. Nhan hai da thuc.\n";
			cin >> n;
		}
		if (n == 1) add_polynomials(poly1, poly2);
		else mul_polynomials(poly1, poly2);
		poly1 = nullptr;
		poly2 = nullptr;
	}
}
int main() {
	option();
	return 1;
}