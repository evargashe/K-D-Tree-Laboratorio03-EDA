#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

const int k = 2; //Dimension

class PuntoKD{
    public:
    double point[k];
    double dist;

        PuntoKD(double p1[k]){
            for (int i = 0; i < k; i++)
                point[i] = p1[i];
        }

        PuntoKD(){}

        void distancia(PuntoKD p1){
            double c1 = 0;

            for (int i = 0; i < k; i++)
                c1 += pow(point[i] - p1.point[i], 2);
    
            dist = sqrt(c1);
        }

        double get_distancia(PuntoKD p1){
            double c1 = 0;

            for (int i = 0; i < k; i++)
                c1 += pow(point[i] - p1.point[i], 2);
    
            return(sqrt(c1));
        }

        void print(){
            for (int i = 0; i < k; i++){
                cout<<point[i]<<" ";
            }
            cout<<endl;
        }

        void set_point(double p1[k]){
            for (int i = 0; i < k; i++)
                point[i] = p1[i];
        }

        bool its_same(PuntoKD p1){
            for (int i = 0; i < k; i++){
                if (point[i] != p1.point[i]){
                    return false;
                }
            }
            return true;
        }
};

class Node{
    PuntoKD punto;
    Node* left;
    Node* right;

    public:
        Node(){
            left = NULL;
            right = NULL;
        }

    friend class KDTREE;
};


class KDTREE{
    private:

    Node* root;

    Node* newNode(double arr[]){
        Node* temp = new Node;

        temp->punto.set_point(arr);
  
        temp->left = temp->right = NULL;
        return temp;
    }

    Node* insertRec(Node* temp, double point[], unsigned depth){
        if (temp == NULL)
            return newNode(point);

        unsigned cd = depth % k;

        if (point[cd] < (temp->punto.point[cd]))
            temp->left  = insertRec(temp->left, point, depth + 1);
        else
            temp->right = insertRec(temp->right, point, depth + 1);

        return temp;
    }

    bool arePointsSame(double point1[], double point2[]){
        for (int i = 0; i < k; ++i)
            if (point1[i] != point2[i])
                return false;
  
        return true;
    }

    bool searchRec(Node* root, double point[], unsigned depth){
        if (root == NULL)
            return false;
        if (arePointsSame(root->punto.point, point))
            return true;
    
        unsigned cd = depth % k;
    
        if (point[cd] < root->punto.point[cd])
            return searchRec(root->left, point, depth + 1);
    
        return searchRec(root->right, point, depth + 1);
    }

    Node* dist_min(double point[], Node* n1, Node* n2){
        PuntoKD p1(point);
        if (p1.get_distancia(n1->punto) <= p1.get_distancia(n2->punto))
            return n1;
        else 
            return n2;
    }

    double dist_cuadrado(double p1[], double p2[]){
        double c1 = 0;

        for (int i = 0; i < k; i++)
            c1 += pow(p1[i] - p2[i], 2);
    
        return c1;
    }

    bool k_in(PuntoKD p1, vector<PuntoKD> arr){
        for (int i = 0; i < arr.size(); i++){
            if (p1.its_same(arr[i])){
                return true;
            }
        }
        return false;
    }

    Node* nn_rec(Node* temp, double target[], unsigned depth){
        if (temp == NULL){
            return temp;
        }

        unsigned cd = depth % k;

        Node* next;
        Node* other;

        if(target[cd] < temp->punto.point[cd]){
            next = temp->left;
            other = temp->right;
        }
        else{
            next = temp->right;
            other = temp->left;
        }

        if(next == NULL)
            return temp;

        Node* temp2 = nn_rec(next, target, depth+1);
        Node* best = dist_min(target, temp2, temp);

        double radio = dist_cuadrado(target, best->punto.point);
        double dist1 = target[cd] - temp->punto.point[cd];


        if ((radio >= pow(dist1,2)) && other != NULL){
            temp2 = nn_rec(other, target, depth+1);
            best = dist_min(target, temp2, best);
            best->punto.print();
        }
        
        return best;
    }

    public:

    KDTREE(){
        root = NULL;
    }

    Node* insert(double point[]){
        if (root == NULL)
            root = newNode(point);

        else{
            Node* temp = root;
            temp = insertRec(temp, point, 0);
        }
    }

    bool search(double point[]){
        return searchRec(root, point, 0);
    }

    void print(){
        if (root == NULL){
            cout<<"null ";
            return;
        }
        root->punto.print();
    }

    void KNN_kdtree(double point[k], int k_n){
        Node* temp = root;
        nn_rec(temp,point,0);
    }
};

void aleatorio(double** array, int n, int cd){
    for (int i = 0; i < n; i++){
        array[i][cd] = rand() % (n/5);
    }
}

void copia(int* arr1, int* arr2, int n){
    arr2 = new int[n];

    for (int i = 0; i < n; i++){
        arr2[i] = arr1[i];
    }
}

void trans_point(string str, double point[k]){
    int n = str.size();
    string num;

    int j = 0, k_ = 0;
    for (int i = 0; i < n; i++){
        if (str[i] == ',' || i == n-1){
            if (i == n-1){
                i++;
            }
            num = str.substr(j,i-j);
            point[k_] = atof(num.c_str());
            k_++;
            j=i+1;
            num.clear();
        }
    }
}

void read_csv( double array[][3]){
    string file = "testX.csv";
    ifstream fin;
    fin.open(file);
    
    string line;

    int i=0;
    while (!fin.eof()) {
        fin>>line;
        trans_point(line, array[i]);
        i++;
    }
    
}

bool comp(PuntoKD a, PuntoKD b){
    return (a.dist < b.dist);
}

void KNN_fb(PuntoKD array[], int n, PuntoKD point, int k_n){
    int* distancia = new int[n-1];

    for (int i = 0; i < n; i++)
        array[i].distancia(point);

    sort(array, array+n, comp);

    for (int i = 0; i < k_n; i++){
        array[i].print();
    }
}


int main(){
    KDTREE kdt;
    double array[3483][3];
    srand(time(NULL));

    read_csv(array);

    PuntoKD puntos[3483];
    for (int i = 0; i < 3483; i++){
        kdt.insert(array[i]);
        puntos[i].set_point(array[i]);
    }

    double test_p[] = {1.60,30,24};
    PuntoKD p1(test_p);

    time_t t_inicio = clock();
    KNN_fb(puntos,3483,p1,100);
    time_t t_final = clock();
    cout<<t_final - t_inicio<<endl;

    t_inicio = clock();
    kdt.KNN_kdtree(test_p,100);
    t_final = clock();
    cout<<t_final - t_inicio<<endl;

    return 0;
}