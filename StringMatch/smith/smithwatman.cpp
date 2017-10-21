#include "StdAfx.h"
#include "smithwatman.h"


smithwatman::smithwatman(void)
{
	//H = 0.0;
	extgap = 0.0;
	opengap=0.0;
	mismatch = 0.0f;

	match = 0.0f;


}

smithwatman::smithwatman(string A,string B,float extgap,float opengap,float mismatch,float match)
{
	smithwatman::A=A;
	smithwatman::B=B;
	smithwatman::extgap=extgap;
	smithwatman::opengap=opengap;


	smithwatman::mismatch=mismatch;
	smithwatman::match=match;
	H.resize(A.length()+1);
	for(int i=0;i<A.length()+1;i++)
		H[i].resize(B.length()+1);


}
smithwatman::~smithwatman(void)
{
}


float smithwatman::S(char a, char b)
{
	if(a==b)
		return match;
	 else
		return mismatch;
}


float smithwatman::W(int k)
{
	return opengap+extgap*k;
}

float max (float a, float b){
	return a > b ? a : b;
}

void smithwatman::Align(void)
{
	    int n=A.length();
		int m=B.length();
		float maxc=0,maxr=0;		
		for(int k=0;k<=n;k++)
			H[k][0]=0;
		for(int l=0;l<=m;l++)
			H[0][l]=0;
		maxc=maxr=0-opengap;
		for(int i=1;i<=n;i++)
			for(int j=1;j<=m;j++)
			{
				float c1=0,c2=0,c3=0;
				 
				c1=H[i-1][j-1]+S(A[i-1],B[j-1]);
				if(maxc-(H[i-1][j]-opengap)>0 )
				  maxc=maxc-extgap;
				else 
				   maxc=H[i-1][j]-opengap-extgap;
				/*for(int k=1;k<=i;k++)
				{
				   c2=H[i-k][j]-W(k);
				   if(c2>maxr)
					   maxr=c2;
				}*/
				 if(maxr-(H[i][j-1]-opengap)>0 )
				  maxr=maxr-extgap;
				else 
				   maxr=H[i][j-1]-opengap-extgap;			 
				
				H[i][j]=max(max(c1, maxc),max(maxr, (float)0.0));	
			}
        for (int k = 0; k <= n; k++) //输出得分矩阵
        {
        	for (int l = 0; l <= m; l++)
				//printf("%f,", H[k][l]);
            cout<<H[k][l]<<",";
			cout<<endl;
        	//System.out.printf("%.1f\t",);
        	//System.out.println();
        }
		
		traceback();
}



void smithwatman::traceback(void)
{      int i,j;
	    string A1,B1,mark;
		float max=0;
		int maxrow=0,maxcol=0;
		float left=0,up=0,diag=0;
		for(i=0;i<=A.length();i++)
			for(j=0;j<=B.length();j++)
				if(H[i][j]>max)
			   {	
					maxrow=i;
				    maxcol=j;
				    max=H[i][j];
			   }
		cout<<"marow:="<<maxrow<<",maxcol="<<maxcol<<endl;		
		
		//A1.insert(0, 1,A[maxrow-1]);	  
		//B1.insert(0, 1,B[maxcol-1]);
		//mark.insert(0,1, '+');
		
		i=maxrow;
		j=maxcol;
		while((i>0)&&(j>0)&&H[i][j]>0)
		{	//cout<<A[i-1]<<"<"<<B[j-1]<<endl;	
			if(fabs(H[i][j]-(H[i-1][j-1]+S(A[i-1],B[j-1])))<=1e-6)
		   {
			    
				 A1.insert(0, 1,A[i-1]);
				 B1.insert(0, 1,B[j-1]);
				 i--;j--;
				 //mark.insert(0, 1,'+');
			}
			else
			{
			 int  k1,k2;			 
			 k1=1;k2=1;
			 int flag=0;
			 while(i-k1>=0||j-k2>=0)
			 {  
			   if(i-k1>=0&&fabs(H[i][j]-(H[i-k1][j]-W(k1)))<=1e-6)
			   { flag=0;
				 break;					  
			   }
			   else
				 k1++;
			   if(j-k2>=0&&fabs(H[i][j]-(H[i][j-k2]-W(k2)))<=1e-6)
			   { 
				 flag=1;
				 break;					//mark.insert(0, 1,'_');
			   }
			   else
				k2++;
			  }
			 cout<<"i="<<i<<"j="<<j<<"k1="<<k1<<"k2="<<k2<<endl;
			 if(flag==0)
			   {
				 for(int m=1;m<=k1;m++)
				 {
					A1.insert(0, 1,A[i-m]);
					B1.insert(0, 1,'-');
				 }
				 i=i-k1;
			  }
			 else
			 {
				 for(int m=1;m<=k2;m++)
				 {
					A1.insert(0, 1,'-');
					B1.insert(0, 1,B[j-m]);
				 }
				 j=j-k2;
			  }

             
		    }
         cout<<A1<<endl;
		 cout<<B1<<endl;
		}
		/*
		for(i=maxrow,j=maxcol;i>0&&j>0;)
			{   
			   	left=H[i][j-1];
			   up=H[i-1][j];
			   diag=H[i-1][j-1];
	  
				if(diag>=up&&diag>=left&&i-1>0&&j-1>0)
				{
					
					if(diag<=0)break;
					i--;j--;
					A1.insert(0, 1,A[i-1]);
					B1.insert(0, 1,B[j-1]);
					mark.insert(0, 1,'+');
					
					
				}
				else if(up>=diag&&up>=left&&i-1>0)
				{
					if(up<=0)break;
					i--;
					A1.insert(0, 1,A[i-1]);
					B1.insert(0, 1,'-');
					mark.insert(0, 1,'_');
				}
				else if(left>=up&&left>=diag&&j-1>0)
				{
					if(left<=0)break;
					j--;
					A1.insert(0,1, '-');
					B1.insert(0, 1,B[j-1]);
					mark.insert(0, 1,'_');
				}
				
				if(i==1&&j==1)break;
				
			}
		*/

		cout<<"原串："<<endl;
		cout<<A<<endl;
		cout<<B<<endl;
	    cout<<A1<<endl;
		cout<<B1<<endl;
		
		/*
		cout<<"最相近的子串："<<endl;
		for(int k=0;k<mark.length();k++)
			if(mark[k]=='+')
				cout<<A1[k];	
				cout<<endl;
        */
		
		

    /*
		for(int k=0;k<mark.length();k++)
			if(mark[k]=='+')
				cout<<B1[k];
				*/
		
}
