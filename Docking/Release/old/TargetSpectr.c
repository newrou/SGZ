#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#define MaxS 40000
#define MaxN 1000
#define R 8.31
#define T0 298.15


typedef struct 
 {
  double G,Gsrc;
  double occupation;
  char *amk[100];
  long n;
 } SpectrLine;

typedef struct 
 {
  double G; // average dG
  double Gmin; // min dG
  double Gf; // gidrofob 
  double occupation;
  char name[300]; // name of segment
  long num[10000]; // number in sequence
  char *amk[10000]; // name of aminoacid
  int sl[10000]; // selected
  long n;
 } Segment;

FILE *flog,*fhtml;
double Tsp=298.15;
double Gs,Ggidrofob;
int HtmlFlag=0;
char *amk[12]={"Arg","Asn","Asp","Gln","Glu","His","Lys","Ser","Thr","Trp","Tyr","H2O"};
SpectrLine *sp[1000000],*src[1000000];
Segment *tg[1000000];
long nsp=0,ntg=0,nsrc=0;
FILE *spf,*tgf;
double Gmax=0.0;
char *Name;

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

double gfamk(char *name)
{
 double x=0.0;
 
 x=0.0;
 if(strncmp(name,"Gly",3)==0) x=0.0;
 if(strncmp(name,"Ala",3)==0) x=-2.88;
 if(strncmp(name,"Val",3)==0) x=-8.64;
 if(strncmp(name,"Leu",3)==0) x=-11.52;
 if(strncmp(name,"Ile",3)==0) x=-11.52;
 if(strncmp(name,"Asp",3)==0) x=-2.88;
 if(strncmp(name,"Asn",3)==0) x=-2.88;
 if(strncmp(name,"Ser",3)==0) x=-2.88;
 if(strncmp(name,"Thr",3)==0) x=-5.76;
 if(strncmp(name,"Cys",3)==0) x=-2.88;
 if(strncmp(name,"Met",3)==0) x=-8.64;
 if(strncmp(name,"Arg",3)==0) x=-8.64;
 if(strncmp(name,"Lys",3)==0) x=-11.52;
 if(strncmp(name,"His",3)==0) x=-11.52;
 if(strncmp(name,"Pro",3)==0) x=-11.52;
 if(strncmp(name,"Phe",3)==0) x=-20.16;
 if(strncmp(name,"Tyr",3)==0) x=-20.16;
 if(strncmp(name,"Trp",3)==0) x=-25.92;
 if(strncmp(name,"Glu",3)==0) x=-5.76;
 if(strncmp(name,"Gln",3)==0) x=-5.76;
 return x;
}

double dG2K(double dG, double T) {return exp(-1000.0*dG/(R*T));}

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

double GibbsSpectrTarget(double T)
{
 long i;
 double Gmin,Ksum,Gs;

 Gmin=tg[0]->G;
 for(i=1;i<ntg;i++) {if(tg[i]->G<Gmin) Gmin=tg[i]->G;}
 Ksum=0.0;
 for(i=0;i<ntg;i++) {tg[i]->occupation=exp(-1000.0*(tg[i]->G-Gmin)/(R*T));Ksum+=tg[i]->occupation;}
 for(i=0;i<ntg;i++) {tg[i]->occupation=tg[i]->occupation/Ksum;} // нормировка
 Gs=0.0; for(i=0;i<ntg;i++) Gs+=tg[i]->occupation*tg[i]->G;
 return(Gs);
}

long ParseSegment(char *s, Segment *t)
{
 long i,j;

 t->n=0;j=0;t->Gf=0;
 for(i=0;s[i]!=' ';i++) {t->name[j]=s[i];j++;}; t->name[j]='\0';
 fprintf(flog,"Segment[%s] ",t->name);
 for(;s[i]!='\0';i++) 
  {
   if(s[i]==' ') {t->num[t->n]=atol(s+i);fprintf(flog," {%ld:%ld",t->n,t->num[t->n]);};
   if(s[i]==':') {t->amk[t->n]=strndup(s+i+1,3);t->Gf+=gfamk(t->amk[t->n]);fprintf(flog," %s:%.2f}",t->amk[t->n],t->Gf);i+=3;t->n++;};
  }
 fprintf(flog,"\n");
 fprintf(flog,"n=%ld Gf=%.2f\n",t->n,t->Gf);
 return t->n;
}

long ReadTarget(FILE *f)
{
 char s[MaxS];

 ntg=0;
 while(!feof(f))
  {
   fgetstr(f,s);
   if(strlen(s)<3) continue;
   fprintf(flog,"\n[%s]\n",s);
   tg[ntg]=(Segment *)malloc(sizeof(Segment));
   ParseSegment(s,tg[ntg]);ntg++;
  }
 fprintf(flog,"ntg=%ld\n",ntg);
 return ntg;
}

long ParseSpectrLine(char *s, SpectrLine *x)
{
 long i,k;
 float f=0,ff=0;

 x->n=0;
 sscanf(s,"%ld",&k);
 sscanf(s+5,"%f",&f);x->G=f-Ggidrofob;x->Gsrc=x->G;
 sscanf(s+15,"%f",&ff);x->occupation=ff;
 fprintf(flog,"[%ld] {%.3f} {%.3f} ",k,x->G,x->occupation);
 for(i=25;s[i]!='\0';i++) 
  {
   if(s[i]=='\t') {x->amk[x->n]=strndup(s+i+1,3);fprintf(flog," [%s]",x->amk[x->n]);i+=3;x->n++;};
  }
 fprintf(flog,"\n");
 fprintf(flog,"n=%ld\n",x->n);
 return x->n;
}

long ReadSpectr(FILE *f)
{
 char s[MaxS];

 nsrc=0;
 fgetstr(f,s);
 while(!feof(f))
  {
   fgetstr(f,s);
   if(strlen(s)<3) continue;
   fprintf(flog,"\n[%s]\n",s);
   src[nsrc]=(SpectrLine *)malloc(sizeof(SpectrLine));
   ParseSpectrLine(s,src[nsrc]);
   nsrc++;
  }
 fprintf(flog,"nsrc=%ld\n",nsrc);
 return nsrc;
}

long FindTarget(long k, char *name)
{
 long i;

 for(i=0;i<tg[k]->n;i++) 
  if(tg[k]->sl[i]==0 && strncmp(name,tg[k]->amk[i],3)==0) {tg[k]->sl[i]=1;return i;};
//  fprintf(flog,"[%s %s]{%ld %ld} \n",name,tg[k]->amk[i],i,tg[k]->n);
 return 0;
}

long SelectSpectr(long k)
{
 long i,j,n;
 double Gs;

 nsp=0;
 fprintf(flog,"\ntarget %s\n",tg[k]->name);
 for(i=0;i<nsrc;i++)
  {
   for(j=0;j<tg[k]->n;j++) tg[k]->sl[j]=0;
   for(j=0;j<src[i]->n;j++) 
    {
     if(strncmp("H2O",src[i]->amk[j],3)==0) continue;
     if(FindTarget(k,src[i]->amk[j])==0) break;
    }
   if(j<src[i]->n) {/*fprintf(flog,"   *** %s [%ld %ld]\n",src[i]->amk[j],nsp,i);*/continue;}
   sp[nsp]=src[i];sp[nsp]->G=sp[nsp]->Gsrc;
   if(tg[k]->Gf > Ggidrofob) sp[nsp]->G+=tg[k]->Gf; else sp[nsp]->G+=Ggidrofob;
   fprintf(flog,"   G=%.3f  Gsrc=%.3f  Gf_tar=%.3f  {%ld %ld}\n",sp[nsp]->G,sp[nsp]->Gsrc,tg[k]->Gf,nsp,i);
   nsp++;
  }
 Gs=GibbsSpectr(Tsp);
 SpectrSort();
 tg[k]->G=Gs;tg[k]->Gmin=sp[0]->G;
 fprintf(flog,"target %s  Gs=%3f  Gmin=%.3f  spn=%ld  nsrc=%ld  n=%ld\n",tg[k]->name,Gs,sp[0]->G,nsp,nsrc,tg[k]->n);
 return nsp;
}


int main(int argc, char *argv[])
{
 int i;
 double Gs,Gmin;
 char *Name,*TargetName;

 HtmlFlag=0;flog=stderr;Name=NULL;Tsp=298.15;Ggidrofob=0.0;Gmax=0.0;
 spf=NULL;tgf=NULL;
 for(i=1;i<argc;i++)
  {
   if(strcmp(argv[i],"-name")==0) {Name=strdup(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-targetname")==0) {TargetName=strdup(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-spectr")==0) {spf=fopen(argv[i+1],"r");Name=strdup(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-target")==0) {tgf=fopen(argv[i+1],"r");TargetName=strdup(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-log")==0) {flog=fopen(argv[i+1],"w");i++;continue;}
   if(strcmp(argv[i],"-html")==0) {HtmlFlag=1;fhtml=fopen(argv[i+1],"w");i++;continue;}
   if(strcmp(argv[i],"-T")==0) {Tsp=atof(argv[i+1]);i++;continue;}
   if(strcmp(argv[i],"-gfob")==0) {Ggidrofob=atof(argv[i+1]);i++;continue;}
  }

 if(HtmlFlag>0) fprintf(fhtml,"<html><title>%s</title><body><h1>%s</h1>\n",Name,Name);
 if(flog==NULL) {fprintf(stderr,"Out file not open!\n");flog=stderr;}
 if(spf==NULL) {fprintf(stderr,"Spectr file not find!\n");return(-1);}
 if(tgf==NULL) {fprintf(stderr,"Target file not find!\n");return(-1);}
 fprintf(flog,"Gmax=%f\n",Gmax);


 ReadSpectr(spf);
 ReadTarget(tgf);
 for(i=0;i<ntg;i++) SelectSpectr(i);
 Gs=GibbsSpectrTarget(Tsp);
 if(HtmlFlag>0) 
  {
   fprintf(fhtml,"<h2>Occupation segments of %s for %s</h2>\n",TargetName,Name);
   fprintf(fhtml,"<table border=2 width=70%%><tr><td>Segment</td><td>dG average, kJ/mol </td><td>K average</td><td>dG min, kJ/mol </td><td>Occupation, %%</td></tr>\n");
   for(i=0;i<ntg;i++) 
    fprintf(fhtml,"<tr><td>%s</td><td>%.3f</td><td>%.3E</td><td>%.3f</td><td>%.2f</td></tr>\n",tg[i]->name,tg[i]->G,dG2K(tg[i]->G,Tsp),tg[i]->Gmin,tg[i]->occupation*100);
   fprintf(fhtml,"</table>\n");
  }

 Gmin=tg[0]->Gmin;
 for(i=1;i<ntg;i++) 
   if(tg[i]->Gmin < Gmin) Gmin=tg[i]->Gmin;

 printf("%.3f",Gs);
 fprintf(flog,"\n\ndG gidrofob = %.3f    ( K = %.3e )\n",Ggidrofob,dG2K(Ggidrofob,Tsp));
 fprintf(flog,"Minimum dG = %.3f kJ/mol   ( K = %.3e )\n",Gmin,dG2K(Gmin,Tsp));
 fprintf(flog,"Average dG = %.3f kJ/mol at %.2f K   ( K = %.3e )\n",Gs,Tsp,dG2K(Gs,Tsp));
 if(HtmlFlag>0) fprintf(fhtml,"<h3>dG gidrofob = %.3f kJ/mol &nbsp;&nbsp; ( K = %.3e )</h3>\n",Ggidrofob,dG2K(Ggidrofob,Tsp));
 if(HtmlFlag>0) fprintf(fhtml,"<h3>Minimum dG = %.3f kJ/mol &nbsp;&nbsp; ( K = %.3e )</h3>\n",Gmin,dG2K(Gmin,Tsp));
 if(HtmlFlag>0) fprintf(fhtml,"<h3>Average dG = %.3f kJ/mol at %.2f K &nbsp;&nbsp; ( K = %.3e )</h3>\n",Gs,Tsp,dG2K(Gs,Tsp));
 if(HtmlFlag>0) {fprintf(fhtml,"</body></html>");fclose(fhtml);}
}
