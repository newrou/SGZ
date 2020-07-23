#include "Molecule-Lib.c"

#define MaxN 1000

char *Head[1000],*HeadS[1000];
long nHead=0,nHeadS=0;
Molecule m;

long ReadNumLine(long *r, char *str)
{
 char *s,*end;
 long i=0,x;

 fprintf(stderr,"    DA s={%s}",str);
 s=strdup(str);i=0;
 while(strlen(s)>0)
  {
   x=strtol(s,&end,10);
   if(s==end) break;
   fprintf(stderr,"  r[%ld]={%ld}",i,x);
   r[i]=x;i++;s=end;
  }
 fprintf(stderr,"  n=%ld\n",i);
 return i;
}

long ReadMolecule(FILE *in, Molecule *mol, char *head[], long *nhead)
{
 char s[MaxS];
 int flag=0;
 long nh,i;
 char *p;
 int Charge;

 nh=0;flag=0;i=0;
 while(!feof(in))
  {
   fgetstr(in,s);
   if(feof(in)) break;
   if(strlen(s)==0) {flag=1;head[nh]=strdup(s);nh++;continue;}
   if(strstr(s,"! Acceptor:")!=NULL) {mol->NAcceptor=ReadNumLine(mol->Acceptor,s+11);continue;}
   if(strstr(s,"! Donor:")!=NULL) {mol->NDonor=ReadNumLine(mol->Donor,s+8);continue;}
   if((p=strstr(s,"ICHARG="))!=NULL) {mol->Charge=atoi(p+7);}
   if(flag==1)
    {
     if(strstr(s,"$END")!=NULL) break;
     if(strstr(s,"$end")!=NULL) break;
     i++; ParseInp(s,&(mol->a[i]));mol->a[i].r=VRFind(mol->a[i].Name);
    }
    else {head[nh]=strdup(s);nh++;}
  }
 mol->NAtom=i;*nhead=nh;
 return mol->NAtom;
}

int main(int argc, char *argv[])
{
 long i,j;
 double gf=0.0;
 FILE *in,*out;

 out=stdout;in=stdin;
 for(j=1;j<argc;j++) 
  {
   if(strcmp(argv[j],"-m")==0) {in=fopen(argv[j+1],"r");j++;continue;}
  }

 ReadMolecule(in,&m,Head,&nHead);

 gf=0;
 for(i=0;i<=m.NAtom;i++) 
  {
   if(strcmp(m.a[i].Name,"C")==0) {gf-=2.88;fprintf(stderr,"C -2.88\n");}
   if(strcmp(m.a[i].Name,"I")==0) {gf-=3.70;fprintf(stderr,"I -3.70\n");}
  }
 printf("%.2f",gf);
 fclose(in);
}
