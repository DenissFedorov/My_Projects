//Рассматриваемая разностная схема будет написана для стационарного уравнения
//теплопроводности (искомая функция не зависит от времени t)

#include <iostream>
#include <cmath>
#include <fstream> 

using namespace std;

float h; //Желаемый шаг сетки
int i;
float razriv, l, u_0, u_1;
float h_1, h_2; //Реальные шаги для отрезка до точки разрыва и после
ofstream ff;

//Коэффициенты, которые можно задавать вручную

float k(float x)
{
	return 1/(x-2);
}

float f(float x)
{
	return -3.0;
}

float q(float x)
{
	return 0.0;
}

//--------------------------------------

float* y_values(int n_1, int n_2){
	float y_max = 0.0;
	float y_l = 0.0;
	float w_h[n_1+n_2+1];

	ff.open("plot_1.txt"); //Сюда будут записываться точки и найденные
	h_1 = razriv/n_1;     // значения в них для gnuplot
    h_2 = (l-razriv)/n_2;

	for (i = 0; i <= n_1+n_2; i++)
	{
		if (i*h_1 <= razriv)
		{	
			w_h[i] = i*h_1;
		}
		else
		{
			w_h[i] = razriv + (i-razriv/h_1)*h_2;
		}
		cout << w_h[i] << endl;
	}; //Точки сетки

	float h_i[n_1+n_2+1], h_h_i[n_1+n_2];
	h_i[0] = 0.0;
	h_h_i[0] = 0.0;

	for (i = 1; i <= n_1+n_2; i++)
	{
		h_i[i] = w_h[i] - w_h[i-1];
		if (i != n_1+n_2) h_h_i[i] = (w_h[i+1] - w_h[i-1])/2;
	}//массивы шагов сетки

	float k_i[2*n_1+2*n_2-1], q_i[n_1+n_2], f_i[n_1+n_2];
	k_i[0] = 0.0;
	q_i[0] = 0.0;
	f_i[0] = 0.0;
	
	for (i = 1; i <= n_1+n_2-1; i++)
	{
		q_i[i] = q(w_h[i]);
		f_i[i] = f(w_h[i]);
		k_i[2*i] = k(w_h[i]+h_i[i+1]/2);
		k_i[2*i-1] = k(w_h[i]-h_i[i]/2);
	}

	//Составляем коэффициенты для метода прогонки
	float a[n_1+n_2], b[n_1+n_2], c[n_1+n_2];
	a[0] = 0.0;
	b[0] = 0.0;
	c[0] = 0.0;
	
	for (i = 1; i <= n_1+n_2-1; i++)
	{
		a[i] = k_i[2*i-1]/(h_i[i]*h_h_i[i]);
		b[i] = k_i[2*i]/(h_i[i+1]*h_h_i[i]);
		c[i] = a[i]+b[i]+q_i[i];
	}

	//Метод прогонки
	//Коэффициенты Ai, Bi, Ci найдены, Fi - в массиве f_i
	//y0 = u_0, yN = u_1
	//Алгоритм правой прогонки взят из книги Самарского
	
	float coef_alpha[n_1+n_2+1], coef_beta[n_1+n_2+1];
	float* total_y = new float[n_1+n_2+1];
	coef_alpha[0] = 0.0;
	coef_alpha[1] = 0.0;
	coef_beta[0] = 0.0;
	coef_beta[1] = u_0;
	total_y[0] = u_0;
	total_y[n_1+n_2] = u_1;

	for (i = 1; i < n_1+n_2; i++)
	{
		coef_alpha[i+1] = b[i]/(c[i]-coef_alpha[i]*a[i]);
		coef_beta[i+1] = (a[i]*coef_beta[i]+f_i[i])/(c[i]-coef_alpha[i]*a[i]);
	}

	//Обратный ход (вычисление yi)
	ff << w_h[n_1+n_2] << ' ' << total_y[n_1+n_2] << endl;	
	for (i = n_1+n_2-1; i >= 1; i--)
	{
		total_y[i] = coef_alpha[i+1]*total_y[i+1]+coef_beta[i+1];
		float t = abs(total_y[i]-w_h[i]*w_h[i]*w_h[i]+13*w_h[i]*w_h[i]-40*w_h[i]);
		if (t > y_max)
		{
			y_max = t;
		}
		y_l += t*t*h_h_i[i];
		ff << w_h[i] << ' ' << total_y[i] << endl;
	}
	ff << 0 << ' ' << total_y[0] << endl;
	ff.close();
	cout << "||y||c: " << y_max << endl;
	cout << "||y||l2: " << y_l << endl;
	
	return total_y;
}
	

int main(){
	float y_max_1, y_max_2, y_max_4;
	int n_1, n_2;
	float* y_1;
    float* y_2;
	float* y_4;
    cout << "Разностная схема для уравнения" << endl;
    cout << "d/dx(k(x)*d/dx*u(x))-q(x)*u(x)=-f(x)" << endl;
    h = 1.0;
    razriv = 2.0;
    l = 5.0;
    //Граничные условия первого рода
    u_0 = 0.0;
    u_1 = 0.0;
	y_max_1 = 0.0;
	y_max_2 = 0.0;
	y_max_4 = 0.0;

	n_1 = razriv/h + 1;
    n_2 = (l-razriv)/h + 1;

	y_1 = y_values(n_1, n_2);
    y_2 = y_values(2*n_1, 2*n_2);
	y_4 = y_values(4*n_1, 4*n_2);	

	return 0;
}
