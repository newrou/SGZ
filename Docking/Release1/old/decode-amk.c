#include <stdio.h>
#include <stdio.h>

typedef struct {
 char rfname[100];
 char name[4];
 char code;
 char efname[100];
 char rname[4];
 } amk;

 amk m[20]=
 {
  {"Глицин","Gly",'G',"Glycine","Гли"},
  {"Аланин","Ala",'A',"Alanine","Ала"},
  {"Валин","Val",'V',"Valine","Вал"},
  {"Изолейцин","Ile",'I',"Isoleucine","Иле"},
  {"Лейцин","Leu",'L',"Leucine","Лей"},
  {"Пролин","Pro",'P',"Proline","Про"},
  {"Серин","Ser",'S',"Serine","Сер"},
  {"Треонин","Thr",'T',"Threonine","Тре"},
  {"Цистеин","Cys",'C',"Cysteine","Цис"},
  {"Метионин","Met",'M',"Methionine","Мет"},
  {"Аспарагиновая кислота","Asp",'D',"asparDic acid","Асп"},
  {"Аспарагин","Asn",'N',"asparagiNe","Асн"},
  {"Глутаминовая кислота","Glu",'E',"gluEtamic acid","Глу"},
  {"Глутамин","Gln",'Q',"Q-tamine","Глн"},
  {"Лизин","Lys",'K',"before L","Лиз"},
  {"Аргинин","Arg",'R',"aRginine","Арг"},
  {"Гистидин","His",'H',"Histidine","Гис"},
  {"Фенилаланин","Phe",'F',"Fenylalanine","Фен"},
  {"Тирозин","Tyr",'Y',"tYrosine","Тир"},
  {"Триптофан","Trp",'W',"tWo rings","Три"}
 };

char *FindName(char c)
{
 int i;

 for(i=0;i<20;i++) if(m[i].code==c) return(m[i].name);
 fprintf(stderr,"!Not Find [%c]\n",c);
 return("!NF");
}

int main()
{
 long i;
 char c;

// for(i=0;i<20;i++) printf("[%c]=[%s]\n",m[i].code,m[i].name);
 printf("All ");i=0;
 while(!feof(stdin))
  {
   c=fgetc(stdin);
   if(c==EOF) continue;
   if(c==' ') continue;
   if(c=='\n') continue;
   if(c=='\013') continue;
   if(c=='\010') continue;
   i++;
   printf("%3ld:%s ",i,FindName(c));
  }
}