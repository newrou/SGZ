#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#define MaxS 40000
#define MaxN 1000
#define R 8.31
#define T0 298.15

char *NameMol;
FILE *flog,*fhtml;
char *PlotName,*Target;
int GibbsType=0;
double G[500][500];
double Gmax=1000.0;
double Tsp=298.15;
char *Name;
long wd1,wa2;
double Gs,Ggidrofob;


typedef struct 
 {
  double T,dE,dH,dG,cv,cp,S,E0,H,G;
  double Hs,Ss,Gs;
  long nB;
  char *Name,*p1,*p2; 
  int pcm; // 1 - pcm; 0 - not pcm
 } ChemTD;

typedef struct 
 {
  long n;
  long x;
  int type; // 0 - acceptor; 1 - donor
  char *Name;
 } Center;

typedef struct 
 {
  double G;
  double occupation;
  long amk[100];
 } SpectrLine;

ChemTD td[10000],tdpcm;
long ntd=0;
int HtmlFlag=0;
char *amk[12]={"Arg","Asn","Asp","Gln","Glu","His","Lys","Ser","Thr","Trp","Tyr","H2O"};
long NCmol=0,NCamk=0;
Center Cmol[500],Camk[500];
SpectrLine *sp[1000000];
long nsp=0;


void fgetstr(FILE *f, char *s)
{
 long n=0;
 while(!feof(f)) 
  {
   s[n]=fgetc(f);
   if(s[n]==13) continue; 
   if(s[n]==10) break; 
   if(s[n]=='\n') break; 
   if(s[n]!=EOF) n++;
   if(n>=MaxS) {s[n]='\0';break;}
  } 
 s[n]='\0';
}

void ParseName(char *Name,char *p1, char *p2)
{
 char *x;
 long n;

 x=index(Name,'+');
 if(x==NULL) {strcpy(p1,Name);strcpy(p2,"");return;}
 strcpy(p2,x+1);n=strlen(Name)-strlen(p2)-1;
 strncpy(p1,Name,n);p1[n]='\0';
}

double dG2K(double dG, double T) {return exp(-1000.0*dG/(R*T));}

void PrintTD_Old(long i)
{
 if(i>=ntd) return;
 fprintf(flog,"%.2f %12.6f %12.6f %12.6f %12.6f %12.6f %12.6f ",td[i].T,td[i].dE,td[i].dH,td[i].dG,td[i].cv,td[i].cp,td[i].S);
 fprintf(flog," %15.6f %15.6f %15.6f %ld [%s] {%s}{%s}",td[i].E0,td[i].H,td[i].G,td[i].nB,td[i].Name,td[i].p1,td[i].p2);
 if(td[i].pcm==1) fprintf(flog," {pcm}"); 
 fprintf(flog,"\n");
}

void PrintTD(long i)
{
// if(i>=ntd) return;
 fprintf(stderr,"%.2f %12.6f %12.6f %12.6f %12.6f %12.6f %12.6f ",td[i].T,td[i].dE,td[i].dH,td[i].dG,td[i].cv,td[i].cp,td[i].S);
 fprintf(stderr," %15.6f %15.6f %15.6f %ld [%s] {%s}{%s}",td[i].E0,td[i].H,td[i].G,td[i].nB,td[i].Name,td[i].p1,td[i].p2);
 if(td[i].pcm==1) fprintf(stderr," {pcm}"); 
 fprintf(stderr,"\n");
}

long ReadTD(FILE *in)
{
 char s[MaxS],E0[MaxS],H[MaxS],G[MaxS],Name[MaxS],nB[MaxS],p1[MaxS],p2[MaxS],Status[MaxS];
 float T,dE,dH,dG,cv,cp,S;
 long len; char *a;

// fgetstr(in,s);
// fgetstr(in,s);
 while(!feof(in))
  {
   fgetstr(in,s);
   if(strlen(s)==0) continue;
   if(strncmp("gas",s,3)!=0) continue;
//   sscanf(s+4,"%f %f %f %f %f %f %f %f %f %s",&T,&dE,&dH,&dG,&cv,&cp,&S,&E0,&H,&G,r);
//   fprintf(flog,"%.2f %12.6f %12.6f %12.6f %12.6f %12.6f %12.6f %12.6f %12.6f %12.6f [%s]\n",T,dE,dH,dG,cv,cp,S,E0,H,G,r);
   sscanf(s+4,"%f %f %f %f %f %f %f %s %s %s %s %s %s",&T,&dE,&dH,&dG,&cv,&cp,&S,E0,H,G,Name,Status,nB);
   td[ntd].T=T;
   td[ntd].dE=dE;
   td[ntd].dH=dH;
   td[ntd].dG=dG;
   td[ntd].cv=cv;
   td[ntd].cp=cp;
   td[ntd].S=S;
   td[ntd].E0=atof(E0);
   td[ntd].H=atof(H);
   td[ntd].G=atof(G);
   td[ntd].pcm=0;
//   if(strstr(Name,"-pcm")!=NULL) {td[ntd].pcm=1;len=strlen(Name);Name[len-4]='\0';}
   if(strstr(Name,"-pcm")!=NULL) {td[ntd].pcm=1;}
   if((a=strstr(Name,".ext"))!=NULL) {a[0]='\0';}
   td[ntd].Name=strdup(Name);
   td[ntd].nB=atol(nB);
   ParseName(Name,p1,p2);
   td[ntd].p1=strdup(p1);
   td[ntd].p2=strdup(p2);
//PrintTD(ntd);
   ntd++;
  }
 fprintf(flog,"\n\n\n");
 return(ntd);
}

long FindNamePCM(char *s)
{
 long i;

 for(i=0;i<ntd;i++)
  {
   if(td[i].pcm!=1) continue;
   if(strcmp(td[i].Name,s)==0) return i;
  }
 return i;
}

long FindNameNoPCM(char *s)
{
 long i;

 for(i=0;i<ntd;i++)
  {
   if(td[i].pcm==1) continue;
   if(strcmp(td[i].Name,s)==0) return i;
  }
 return i;
}

long FindName(char *s)
{
 long i;

 for(i=0;i<ntd;i++)
  {
   if(strcmp(td[i].Name,s)==0) return i;
  }
 return i;
}

long FindFormat(char *fmt, ...)
{
 va_list arg;
 char Name[MaxS];

 va_start(arg,fmt);
 vsprintf(Name,fmt,arg);
 va_end(arg);
 return FindName(Name);
}

long FindTDpcm()
{
 long i,y;

 fprintf(flog,"\nFind pcm TD correction");
 for(i=0;i<ntd;i++)
  {
   if(td[i].pcm==1) continue;
   y=FindFormat("%s-pcm",td[i].Name);
   if((td[y].S < 0.0001) && y<ntd)
    {
     fprintf(flog,"  Copy TD correction [%s] -> [%s]\n",td[i].Name,td[y].Name);
     td[y].T=td[i].T;
     td[y].dE=td[i].dE;
     td[y].dH=td[i].dH;
     td[y].dG=td[i].dG;
     td[y].cv=td[i].cv;
     td[y].cp=td[i].cp;
     td[y].S=td[i].S;
     td[y].H=td[y].E0+td[y].dH;
     td[y].G=td[y].E0+td[y].dG;
     fprintf(flog,"    E0=%f  G=%f  H=%f\n",td[y].E0,td[y].G,td[y].H);
    }
  }
 fprintf(flog,"\n");
}

double EvalDelta2(long x, long y)
{
 double S,H,G,E;

 H=td[x].H-td[y].H;
 S=td[x].S-td[y].S;
 G=td[x].G-td[y].G;
 E=td[x].E0-td[y].E0;
 fprintf(flog," dH=%f  dS=%f  dG=%f  dE=%f\n",H,S,G,E);
 return(G);
}

double EvalDelta3(long x, long y, long z)
{
 double S,H,G,E;

 H=td[x].H-td[y].H-td[z].H;
 S=td[x].S-td[y].S-td[z].S;
 G=td[x].G-td[y].G-td[z].G;
 E=td[x].E0-td[y].E0-td[z].E0;
 fprintf(flog," dH=%f  dS=%f  dG=%f  dE=d[i%f\n",H,S,G,E);
 return(G);
}

double dH(long x, long y) {return(td[x].H-td[y].H);}
double dS(long x, long y) {return(td[x].S-td[y].S);}
double dG(long x, long y) {return(td[x].G-td[y].G);}
double dE(long x, long y) {return(td[x].E0-td[y].E0);}

double dH3(long x, long y, long z) {return(td[x].H-td[y].H-td[z].H);}
double dS3(long x, long y, long z) {return(td[x].S-td[y].S-td[z].S);}
double dG3(long x, long y, long z) {return(td[x].G-td[y].G-td[z].G);}
double dE3(long x, long y, long z) {return(td[x].E0-td[y].E0-td[z].E0);}

double dH4(long p1, long p2, long i1, long i2) {return(td[p1].H+td[p2].H-td[i1].H-td[i2].H);}
double dS4(long p1, long p2, long i1, long i2) {return(td[p1].S+td[p2].S-td[i1].S-td[i2].S);}
double dG4(long p1, long p2, long i1, long i2) {return(td[p1].G+td[p2].G-td[i1].G-td[i2].G);}
double dE4(long p1, long p2, long i1, long i2) {return(td[p1].E0+td[p2].E0-td[i1].E0-td[i2].E0);}

void sPrintTD(long i) {if(i<ntd) fprintf(flog," dH=%f  dS=%f  dG=%f  [%s]\n",td[i].H,td[i].S,td[i].G,td[i].Name);}

double GibbsR(double T, long n, double *E, double *X, FILE *f, char *Name)
{
 long i;
 double Emin,K[n],Ksum,Xg;
 FILE *PlotOut;
 char FName[MaxS];

 Emin=E[1];
 for(i=2;i<=n;i++) if(E[i]<Emin) Emin=E[i];
 Ksum=0.0;
 for(i=1;i<=n;i++) {K[i]=exp(-1000.0*(E[i]-Emin)/(R*T));Ksum+=K[i];}
 for(i=1;i<=n;i++) {K[i]=K[i]/Ksum;fprintf(f,"K[%ld]=%f  dX[%ld]=%f\n",i,K[i],i,K[i]*X[i]);} // нормировка
 fprintf(f,"\n",Ksum);
 Xg=0;
 for(i=1;i<=n;i++) Xg+=K[i]*X[i];
 if(PlotName!=NULL) 
  {
   sprintf(FName,"%s-%s.plot",PlotName,Name);
   PlotOut=fopen(FName,"w");
   for(i=1;i<=n;i++) fprintf(PlotOut,"%ld %f %f %f %f\n",i,E[i],X[i],K[i],Xg);
   fclose(PlotOut);
  }
 return(Xg);
}

double fs(double E, long ns, double *Es, double *Xs)
{
 long i,min,max;
 double Emin,Emax,r;

 Emin=Es[1];Emax=Es[1];min=1;max=1;
 for(i=2;i<=ns;i++) {if(Es[i]<Emin) {Emin=Es[i];min=i;} if(Es[i]>Emax) {Emax=Es[i];max=i;}}
 for(i=2;i<=ns;i++)
  {
   if(Es[i]<=E)
    if(Es[i]>=Emin) {Emin=Es[i];min=i;}
   if(Es[i]>=E)
    if(Es[i]<=Emax) {Emax=Es[i];max=i;}
  }
 if(min==max) r=Xs[min]; else r=Xs[min]+(Xs[max]-Xs[min])*(E-Emin)/(Emax-Emin);
 return r;
}

double GibbsMax(double T, long ns, double *Es, double *Xs, FILE *f)
{
 long i,n=100000;
 double Emin,Emax,K[100001],E[100001],Ksum,Xg;

 Emin=Es[1];Emax=Es[1];
 for(i=2;i<=ns;i++) {if(Es[i]<Emin) Emin=Es[i]; if(Es[i]>Emax) Emax=Es[i];}

 for(i=1;i<=n;i++) E[i]=Emin+(Emax-Emin)*(i-1)/(n-1);

 Ksum=0.0;
 for(i=1;i<=n;i++) {K[i]=exp(-1000.0*(E[i]-Emin)/(R*T));Ksum+=K[i];}
 for(i=1;i<=n;i++) {K[i]=K[i]/Ksum;} // нормировка
 Xg=0;
 for(i=1;i<=n;i++) {Xg+=K[i]*fs(E[i],ns,Es,Xs);}
 return(Xg);
}

double Gibbs(double T, long n, double *E, double *X, FILE *f, char *Name)
{if(GibbsType==0) return(GibbsR(T,n,E,X,f,Name)); else return(GibbsMax(T,n,E,X,f));}

void CopyTD(ChemTD *r, ChemTD *x) {memcpy(r,x,sizeof(ChemTD));}

long AddCmol(long n,char *s, int t,long x) 
 {Cmol[NCmol].n=n;Cmol[NCmol].x=x;Cmol[NCmol].type=t;Cmol[NCmol].Name=strdup(s); return(NCmol++);}
long AddCamk(long n,char *s, int t,long x) 
 {Camk[NCamk].n=n;Camk[NCamk].x=x;Camk[NCamk].type=t;Camk[NCamk].Name=strdup(s); return(NCamk++);}

long PrintCenters()
{
 long i;
 for(i=0;i<NCmol;i++) fprintf(flog,"%ld -> %ld [%d] %s \n",Cmol[i].x,Cmol[i].n,Cmol[i].type,Cmol[i].Name);
 for(i=0;i<NCamk;i++) fprintf(flog,"%ld -> %ld [%d] %s \n",Camk[i].x,Camk[i].n,Camk[i].type,Camk[i].Name);
 return NCmol+NCamk;
}

long FindMolCenter(char *Name)
{
 long i,j,x1,x2;
 for(i=0;i<500;i++)
  {
   x1=FindFormat("%s-a%ld+H2O-d1-pcm",Name,i);
   x2=FindFormat("%s-d%ld+H2O-a2-pcm",Name,i);
   if(x1<ntd) {fprintf(flog,"%s-a%ld\n",Name,i);AddCmol(x1,Name,0,i);}
   if(x2<ntd) {fprintf(flog,"%s-d%ld\n",Name,i);AddCmol(x2,Name,1,i);}
  }
 for(j=0;j<12;j++)
  for(i=0;i<100;i++)
   {
    x1=FindFormat("H2O-d1+%s-a%ld-pcm",amk[j],i);
    x2=FindFormat("H2O-a2+%s-d%ld-pcm",amk[j],i);
    if(x1<ntd) {fprintf(flog,"%s-a%ld\n",amk[j],i);if(strcmp(amk[j],"H2O")==0) wd1=NCamk;AddCamk(x1,amk[j],0,i);}
    if(x2<ntd) {fprintf(flog,"%s-d%ld\n",amk[j],i);if(strcmp(amk[j],"H2O")==0) wa2=NCamk;AddCamk(x2,amk[j],1,i);}
   }
}

char fType(int c) {if(c==0) return 'a'; else return 'd';}

void MakeLine(SpectrLine *s, long n)
{
 long i;
 double Gsum=0;

 if(n<NCmol) 
  {
   for(i=0;i<NCamk;i++) 
    {
     if(Cmol[n].type==Camk[i].type) continue;
     if(G[n][i]==0.0 && strcmp(Camk[i].Name,"H2O")!=0 ) {/*printf("skip[%ld,%ld] \n",n,i);*/continue;}
     s->amk[n]=i;MakeLine(s,n+1);
    }
  }
  else 
  {
    Gsum=0;
    for(i=0;i<NCmol;i++) Gsum+=G[i][s->amk[i]];
    if(Gsum < Gmax) 
     {
      if(nsp<1000000)
       {
        sp[nsp]=(SpectrLine *)malloc(sizeof(SpectrLine));
        sp[nsp]->G=Gsum+Ggidrofob;
        sp[nsp]->occupation=0.0;
        //printf("%8.3f",Gsum);
        ///////////for(i=0;i<NCmol;i++) printf("\t%s(%.3f) ",Camk[s->amk[i]].Name,G[i][s->amk[i]]);
        for(i=0;i<NCmol;i++) {/*printf("\t%s",Camk[s->amk[i]].Name);*/sp[nsp]->amk[i]=s->amk[i];}
        //printf("\n");
        nsp++;
       }
       else printf("!!! Number of line > 1000000 \n");
     }
  }
}

void SpectrSort()
{
 long i,j;
 double Gmin;
 SpectrLine *x;

 for(j=0;j<nsp;j++)
  {
   for(i=j;i<nsp;i++) 
    if(sp[j]->G > sp[i]->G) {x=sp[j];sp[j]=sp[i];sp[i]=x;}
  }
}

void PrintSpectr(FILE *spout)
{
 long i,j;

 fprintf(spout,"   N\t   dG   \t Occupation");for(i=0;i<NCmol;i++) fprintf(spout,"\t%c%ld",fType(Cmol[i].type),Cmol[i].x);fprintf(spout,"\n");
 for(j=0;j<nsp;j++)
  {
   fprintf(spout,"%4ld\t%8.3f\t  %8.3f  ",j+1,sp[j]->G,sp[j]->occupation);
   for(i=0;i<NCmol;i++) fprintf(spout,"\t%s",Camk[sp[j]->amk[i]].Name);
   fprintf(spout,"\n");
  }
 if(HtmlFlag>0) 
  {
   fprintf(fhtml,"<h2>Thermodynamic spectrum of the interaction of %s with peptides</h2>\n",Name);
   fprintf(fhtml,"<table border=2 width=70%%><tr><td>dG</td><td>Occupation</td><td>K");for(i=0;i<NCmol;i++) fprintf(fhtml,"</td><td>%c%ld",fType(Cmol[i].type),Cmol[i].x);fprintf(fhtml,"</td></tr>\n");
   for(j=0;j<nsp;j++)
   {
    fprintf(fhtml,"<tr><td>%8.3f</td><td>%8.3f<td>%.3e",sp[j]->G,sp[j]->occupation,dG2K(sp[j]->G,Tsp));
    for(i=0;i<NCmol;i++) fprintf(fhtml,"</td><td>%s",Camk[sp[j]->amk[i]].Name);
    fprintf(fhtml,"</td></tr>\n");
   }
   fprintf(fhtml,"</table>\n\n");
  }
}

double GibbsSpectr(double T)
{
 long i;
 double Gmin,Ksum,Gs;

 Gmin=sp[0]->G;
 for(i=1;i<nsp;i++) {if(sp[i]->G<Gmin) Gmin=sp[i]->G;}
 Ksum=0.0;
 for(i=0;i<nsp;i++) {sp[i]->occupation=exp(-1000.0*(sp[i]->G-Gmin)/(R*T));Ksum+=sp[i]->occupation;}
 for(i=0;i<nsp;i++) {sp[i]->occupation=sp[i]->occupation/Ksum;} // нормировка
 Gs=0.0; for(i=0;i<nsp;i++) Gs+=sp[i]->occupation*sp[i]->G;
 return(Gs);
}

long MakeSpectr()
{
 long i,j,n,w,x1,x2,xmol,xamk,dHB,dHB2,NL;
 SpectrLine Line;

 if(HtmlFlag>0) 
  {
   fprintf(fhtml,"<h2>Interaction can increase the strength of the complex for %s</h2>\n",Name);
   fprintf(fhtml,"<table border=2 width=70%%><tr><td>dG</td><td>K</td><td>Molecule center</td><td>Aminoacid center</td></tr>\n");
  }
 NL=0;fprintf(flog,"\n");
 w=FindFormat("2H2O-pcm");
 for(i=0;i<NCmol;i++) 
  for(j=0;j<NCamk;j++) 
   {
    G[i][j]=0.0;
    n=FindFormat("%s-%c%ld+%s-%c%ld-pcm",Cmol[i].Name,fType(Cmol[i].type),Cmol[i].x,Camk[j].Name,fType(Camk[j].type),Camk[j].x);
    if(n<ntd) 
     {
      x1=Cmol[i].n;
      x2=Camk[j].n;
      xmol=FindFormat("%s-pcm",Cmol[i].Name);
      xamk=FindFormat("%s-pcm",Camk[j].Name);
      G[i][j]=dG4(n,w,x1,x2);
      dHB=td[n].nB+td[w].nB-td[x1].nB-td[x2].nB;
      if(dHB!=0) {G[i][j]=0.0;continue;fprintf(flog,"    * ");}
//      if(abs(dHB)>1) {G[i][j]=0.0;continue;fprintf(flog,"    * ");}
      if(fabs(G[i][j])>1000) {G[i][j]=0.0;continue;fprintf(flog," *** ");}
      if(G[i][j]>0) {G[i][j]=0.0;continue;fprintf(flog," *** ");}
      fprintf(flog,"G[%ld][%ld]=%7.3f     K = %.3e",i,j,G[i][j],dG2K(G[i][j],Tsp));
      fprintf(flog,"%s-%c%ld+%s-%c%ld [%ld] ",Cmol[i].Name,fType(Cmol[i].type),Cmol[i].x,Camk[j].Name,fType(Camk[j].type),Camk[j].x,dHB);
      fprintf(flog,"{%ld %ld   %ld %ld   %ld %ld} \n",td[xmol].nB,td[x1].nB,td[xamk].nB,td[x2].nB,td[n].nB,td[w].nB);
      if(HtmlFlag>0 && G[i][j]<0) fprintf(fhtml,"<tr><td>%7.3f</td><td>%.3e</td><td>%s-%c%ld</td><td>%s-%c%ld</td></tr>\n",G[i][j],dG2K(G[i][j],Tsp),Cmol[i].Name,fType(Cmol[i].type),Cmol[i].x,Camk[j].Name,fType(Camk[j].type),Camk[j].x);
      if(dHB!=0) {G[i][j]=0.0;continue;}
      if(fabs(G[i][j])>1000) {G[i][j]=0.0;continue;}
      NL++;
     }
   }
 if(HtmlFlag>0) fprintf(fhtml,"</table>\n\n");
 fprintf(flog,"NL=%ld\n",NL);
 for(i=0;i<NCmol;i++) fprintf(flog,"*** mol center %ld [%s] %ld %c\n",i,Cmol[i].Name,Cmol[i].x,fType(Cmol[i].type));
 for(j=0;j<NCamk;j++) fprintf(flog,"*** amk center %ld [%s] %ld %c\n",j,Camk[j].Name,Camk[j].x,fType(Camk[j].type));
 //printf("   dG\t");for(i=0;i<NCmol;i++) printf("\t%c%ld",fType(Cmol[i].type),Cmol[i].x);printf("\n");
 MakeLine(&Line,0);

 sp[nsp]=(SpectrLine *)malloc(sizeof(SpectrLine));
 sp[nsp]->G=0.0; sp[nsp]->occupation=0.0;
 for(i=0;i<NCmol;i++) if(fType(Cmol[i].type)=='a') sp[nsp]->amk[i]=wd1; else sp[nsp]->amk[i]=wa2;
 nsp++;

 Gs=GibbsSpectr(Tsp);
 SpectrSort();
 PrintSpectr(stdout);
 fprintf(flog,"\nAverage at %.2f K   G = %.3f   K = %.3e \n",Tsp,Gs,dG2K(Gs,Tsp));
 return NCmol+NCamk;
}

int main(int argc, char *argv[])
{
 FILE *in,*amk,*f;
 ChemTD r;
 int i;
 char *SName,str[MaxS];
 int stype=-1,gt=0;
 char *ns,*react;
 char s[MaxS];

 ntd=0;HtmlFlag=0;flog=stderr;stype=0;Name=NULL;stype=-1;ns=NULL;gt=0;react=NULL;PlotName=NULL;Gmax=1000.0;Tsp=298.15;Ggidrofob=0.0;
 in=stdin;amk=stdin;
 for(i=1;i<argc;i++)
  {
   if(strcmp(argv[i],"-dat")==0) {in=fopen(argv[i+1],"r");i++;continue;}
   if(strcmp(argv[i],"-amk")==0) {amk=fopen(argv[i+1],"r");i++;continue;}
   if(strcmp(argv[i],"-log")==0) {flog=fopen(argv[i+1],"w");i++;continue;}
   if(strcmp(argv[i],"-html")==0) {HtmlFlag=1;fhtml=fopen(argv[i+1],"w");i++;continue;}
   if(strcmp(argv[i],"-name")==0) {Name=strdup(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-plot")==0) {PlotName=strdup(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-type")==0) {stype=atoi(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-T")==0) {Tsp=atof(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-Gmax")==0) {Gmax=atof(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-gmax")==0) {Gmax=atof(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-gfob")==0) {Ggidrofob=atof(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-ns")==0) {ns=strdup(argv[i+1]);i++;continue;}
  }

 if(HtmlFlag>0) fprintf(fhtml,"<html><title>%s</title><body><h1>%s</h1>\n",Name,Name);
 if(flog==NULL) {fprintf(stderr,"Out file not open!\n");flog=stderr;}
 if(Name==NULL) {fprintf(stderr,"Name or reaction not find!\n");return(-1);}
 if(in==NULL) {fprintf(stderr,"TD file not find!\n");return(-1);}
 if(amk==NULL) {fprintf(stderr,"TD file not find!\n");return(-1);}
 fprintf(flog,"Gmax=%f\n",Gmax);


 ReadTD(in);
 ReadTD(amk);
 FindTDpcm();
 FindMolCenter(Name);
 PrintCenters();
 MakeSpectr();
 fprintf(flog,"dG gidrofob = %.3f    ( K = %.3e )\n",Ggidrofob,dG2K(Ggidrofob,Tsp));
 fprintf(flog,"Minimum dG = %.3f kJ/mol   ( K = %.3e )\n",sp[0]->G,dG2K(sp[0]->G,Tsp));
 fprintf(flog,"Average dG = %.3f kJ/mol at %.2f K   ( K = %.3e )\n",Gs,Tsp,dG2K(Gs,Tsp));
 if(HtmlFlag>0) 
  {
   fprintf(fhtml,"<h3>dG gidrofob = %.3f kJ/mol &nbsp;&nbsp; ( K = %.3e )</h3>\n",Ggidrofob,dG2K(Ggidrofob,Tsp));
   fprintf(fhtml,"<h3>Minimum dG = %.3f kJ/mol &nbsp;&nbsp; ( K = %.3e )</h3>\n",sp[0]->G,dG2K(sp[0]->G,Tsp));
   fprintf(fhtml,"<h3>Average dG = %.3f kJ/mol at %.2f K &nbsp;&nbsp; ( K = %.3e )</h3>\n",Gs,Tsp,dG2K(Gs,Tsp));
   fprintf(fhtml,"<img src=images/Spectr1.gif width=80%%><br>\n<img src=images/Spectr2.gif width=80%%><br>\n");
   fprintf(fhtml,"<img src=images/Spectr3.gif width=80%%><br>\n<img src=images/Spectr4.gif width=80%%><br>\n</body></html>");fclose(fhtml);
  }
}
