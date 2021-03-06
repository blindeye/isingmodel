#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"twist.c"

typedef struct conf{
		int spin;
		int up;
		int down;
		int left;
		int right;
}Ising;

void initialize(int N, Ising **S);
void mcstep(int N, double T, Ising **S);
double magnetization(int N, Ising **S);
double energy(int N, Ising **S);

int main(int argc, char **argv)
{
		double T=0;;
		double mag=0.0;
		int N;
		int i;
		Ising **S;
		double tot_Erg=0;
		double capacity=0.0;
		double kai=0.0; 

		init_rnd(gus());

		N=atoi(argv[1]);
		S=(Ising **)malloc(sizeof(Ising *) * N);
		for(i=0; i<N; i++)
				S[i]=(Ising *)malloc(sizeof(Ising) * N);

		initialize(N, S);

		for(T=3.01; T>=0.01; T-=0.01)
		{
				mag=0.0;
				tot_Erg=0.0;
				kai=0.0;
				capacity=0.0;

				for(i=0; i<200000; i++)
						mcstep(N, T, S);

				for(i=0; i<200000; i++)
				{
						mcstep(N, T, S);
						mag+=magnetization(N, S);
						tot_Erg+=energy(N, S);
						kai+=pow(magnetization(N, S), 2);
						capacity+=pow(energy(N,S), 2);
				}
				mag=mag/200000;
				tot_Erg=tot_Erg/200000;
				kai=kai/200000;
				capacity=capacity/200000;

				kai=(kai-mag*mag)/T;
				capacity=(capacity - tot_Erg*tot_Erg)/(T*T);

				kai=kai*N*N;
				capacity=capacity*N*N;

				printf("%lf\t%lf\t%lf\t%lf\t%lf\n", T, mag, tot_Erg, kai, capacity);
		}

		free(S);

		return 0;

}

void initialize(int N, Ising **S)
{
		int i,j;

		for(i=0; i<N; i++)
				for(j=0; j<N; j++)
				{
						if(drnd() < 0.5)
								S[i][j].spin=1;
						else
								S[i][j].spin=-1;
				}

		for(i=0; i<N; i++)
				for(j=0; j<N; j++)
				{
						S[i][j].up=i-1;
						S[i][j].down=i+1;
						S[i][j].left=j-1;
						S[i][j].right=j+1;
				}

		for(j=0; j<N; j++)
		{
				S[0][j].up=N-1;
				S[N-1][j].down=0;
		}

		for(i=0; i<N; i++)
		{
				S[i][0].left=N-1;
				S[i][N-1].right=0;
		}
}

void mcstep(int N, double T, Ising **S)
{
		int i,j,k;
		double E_flip;
		double E1, E2;

		for(k=0; k<N*N; k++)
		{
				i=(int)(drnd()*N);
				j=(int)(drnd()*N);
				E1=-S[i][j].spin * (S[ S[i][j].up ][j].spin + S[ S[i][j].down ][j].spin + S[i][ S[i][j].left ].spin + S[i][ S[i][j].right ].spin );
				E2=-(-S[i][j].spin) * (S[ S[i][j].up ][j].spin + S[ S[i][j].down ][j].spin + S[i][ S[i][j].left ].spin + S[i][ S[i][j].right ].spin );

				E_flip=E2-E1;

				if(E_flip<0 || drnd() < exp(-E_flip/T))
						S[i][j].spin=-S[i][j].spin;
		}
}

double magnetization(int N, Ising **S)
{
		int i, j;
		double mag=0.0;

		for(i=0; i<N; i++)
				for(j=0; j<N; j++)
						mag+=S[i][j].spin;

		mag=mag/(N*N);

		return fabs(mag);
}

double energy(int N, Ising **S)
{
		int i, j;
		double E_tot;
		E_tot=0.0;

		for(i=0; i<N; i++)
				for(j=0; j<N; j++)
						E_tot+=-S[i][j].spin * ( S[ S[i][j].up ][j].spin + S[ S[i][j].down ][j].spin + S[i][ S[i][j].left ].spin + S[i][ S[i][j].right ].spin );

		E_tot=E_tot/(N*N);

		return E_tot;
}

