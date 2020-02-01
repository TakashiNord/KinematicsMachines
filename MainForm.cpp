//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "MainForm.h"
#include "MessageForm.h"
#include "about.h"

#include "KinematicsMachines_utils.h"

#define COUNT_MACHINE 18

struct MACHINES
{
   int num;
   int methods;
   char name[30];
   int ai, bj, ck;
   char A[10], B[10], C[10];
   double minai, maxai;
   double minbj, maxbj;
   double minck, maxck;
} ;

static struct MACHINES machine_list[COUNT_MACHINE] =
{
 { 0,99,"Cферическиe",1,1,0, "Theta","Phi","r",0,180,0,360,0,0}, // первая строка зарезервирована для Сферического преоб
 { 1, 7,"MAG3ex",1,0,1,"A","B","C",-110,110,0,0,-360,360},
 { 2, 0,"Variaxis-630",1,0,1,"A","B","C",-120,30,0,0,-360,360},
 { 3, 1,"DMU-50ev",0,1,1,"A","B","C",0,0,0,180,-360,360},
 { 4, 0,"DMU-125PA",1,0,1,"A","B","C",-120,10,0,0,-360,360},
 { 5, 2,"DMU-125PB",0,1,1,"A","B","C",0,0,0,180,-360,360},
 { 6, 2,"DMU-200PT",0,1,1,"A","B","C",0,0,0,180,-360,360},
 { 7, 3,"V-STAR (+POGO)",0,1,1,"A","B","C",0,0,-110,110,-400,400},
 { 8, 3,"BФ-5BC",0,1,1,"A","B","C",0,0,-110,110,-360,360},
 { 9, 4,"FSQV-80KR",1,1,0,"A","B","C",-180,30,-90,90,0,0},
 {10, 4,"MA655c5",1,1,0,"A","B","C",-21.5,21.5,-21.5,21.5,0,0},
 {11, 4,">ФП-14В7",1,1,0,"A","B","C",-21.5,21.5,-21.5,21.5,0,0},
 {12, 3,"NC-1325IP",0,1,1,"A","B","C",0,0,-105,105,-200,200},
 {13, 5,"PФП-6K",1,0,1,"A","B","C",-30,30,0,0,-270,270},
 {14, 6,"AGP630-800b",0,1,0,"A","B","C",0,0,-360,360,0,0},
 {15, 3,"DMU-35M",0,1,1,"A","B","C",0,0,-15,105,-360,360},
 {16, 8,"Kлеп. авт. Ipac",1,0,0, "A","B*","E*",-180,180,-12,12,-360,360},
 {17,-1,"none",0,0,0, "A","B","C",0,0,0,0,0,0}
};

/* This function read the numeric string after key letter string
  in success it return 1, otherwice 0
*/
int  read_str(char *s,char *w,char *sw)
{
   char *word,sym;
   int  n,m;  int fl_break;
   if(s==NULL) return(0);
   if ((word=strstr(s,w))==NULL) return(0);
   m=n=strlen(w);
   fl_break=0;
   do
   {  sym=*(word+n);
      if (isdigit(sym)) { fl_break++; n++; }
       else if (sym==' ' && fl_break!=0) break;
              else if (sym==' '||sym=='+'||sym=='-'||sym=='.') n++;
                    else break;
   } while (1);
   if (n>0){ n-=m; strncpy(sw,word+m,n); sw[n]='\0'; return(n); }
   else                                        return(0);
}

/*Удаление кодов*/
int delete_str(char *s,char *w)
{
 int n,m,i;
 char old_sw[50],save[256],*p;

 if(s==NULL) return (0);

 n=0;
 if ((n=read_str(s,w,old_sw))!=0)
  {
    p=strstr(s,w);
    if (p) {
     i=p-s;
     m=n+strlen(w);
     while(m!=0) {
       s[i]=' ';
       m--; i++ ;
     }

    }
  }

 return n;
}



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
    ListMachine =  new TStringList();
    ListDecriptions =  new TStringList();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OutputMessage(AnsiString s)
{
  FormMessage->Memo1->Lines->Append(s);
  if (CheckBoxMesForm->Checked==False) {
      FormMessage->Visible=True;// FormMessage->Show();
  }
  return;
}

//---------------------------------------------------------------------------

/***********************************************************************/
/***************************
int Machine = список загруженных станков из списка
int Methods = методы 0=ABC->ijk ..=ijk->ABC
********************************************/
int __fastcall  TFormMain::DecideMachine(int Machine, int Methods)
{
  AnsiString Strmach , str;
  double ang[12], ijk[3], r ;
  long int i ;
  char mes[256] ;

/***********************************************************************************************************/

 /* mes[0]='\0'; sprintf(mes,"Machine=%d, Methods=%d ", Machine, Methods); MessageBox(NULL,mes,"DecideMachine",MB_OK);  */

  for(i=0;i<12;i++) ang[i]=0.;
  for(i=0;i<3;i++) ijk[i]=0.; ijk[2]=1.;

 // проверка и установка значений.
  switch (Methods)
  {
   case 0 :  // A,B,C->i,j,k

       str=E1->Text; ang[0]=atof(str.c_str());
       str=E2->Text; ang[1]=atof(str.c_str());
       str=E3->Text; ang[2]=atof(str.c_str());

       mes[0]='\0'; sprintf(mes,"%.5lf",ang[0]);   E1->Text=mes;
       mes[0]='\0'; sprintf(mes,"%.5lf",ang[1]);   E2->Text=mes;
       mes[0]='\0'; sprintf(mes,"%.5lf",ang[2]);   E3->Text=mes;

   break ;
   default :

       str=E4->Text; ijk[0]=atof(str.c_str());
       str=E5->Text; ijk[1]=atof(str.c_str());
       str=E6->Text; ijk[2]=atof(str.c_str());

       r=sqrt(ijk[0]*ijk[0]+ijk[1]*ijk[1]+ijk[2]*ijk[2]);
       if (EQ_is_zero(r)) {
         ijk[0]=0.; ijk[1]=0.; ijk[2]=1.; r=1.;
         OutputMessage("Проверка ввода :");
         OutputMessage("\t!! вектор={i,j,k} - нулевой.");
       }
       if (!EQ_is_equal(r,1.000)) {
            OutputMessage("Проверка ввода :");
            mes[0]='\0'; sprintf(mes,"\t !! вектор={i,j,k} - не нормирован->нормируем... koef=%.3lf ",1./r);
            OutputMessage(mes);
            ijk[0]/= r; ijk[1]/= r; ijk[2]/= r;
       }

       mes[0]='\0'; sprintf(mes,"%.8lf",ijk[0]);  E4->Text=mes;
       mes[0]='\0'; sprintf(mes,"%.8lf",ijk[1]);  E5->Text=mes;
       mes[0]='\0'; sprintf(mes,"%.8lf",ijk[2]);  E6->Text=mes;

  }

  Strmach=ListMachine->Strings[Machine]+ ">" ;

/***********************************************************************************************************/


  if (machine_list[Machine].ai) {
    if (ang[0]<machine_list[Machine].minai || ang[0]>machine_list[Machine].maxai)
    {
      mes[0]='\0'; sprintf(mes,"! %s %.1lf°..%.1lf°",machine_list[Machine].A,machine_list[Machine].minai,machine_list[Machine].maxai);
      OutputMessage(Strmach + mes);
    }
  }
  if (machine_list[Machine].bj) {
    if (ang[1]<machine_list[Machine].minbj || ang[1]>machine_list[Machine].maxbj)
    {
      mes[0]='\0'; sprintf(mes,"! %s %.1lf°..%.1lf°",machine_list[Machine].B,machine_list[Machine].minbj,machine_list[Machine].maxbj);
      OutputMessage(Strmach + mes);
    }
  }
  if (machine_list[Machine].ck) {
    if (ang[2]<machine_list[Machine].minck || ang[2]>machine_list[Machine].maxck)
    {
      mes[0]='\0'; sprintf(mes,"! %s %.1lf°..%.1lf°",machine_list[Machine].C,machine_list[Machine].minck,machine_list[Machine].maxck);
      OutputMessage(Strmach + mes);
    }
  }

  //OutputMessage(Strmach + "-----------------------------in-----------------------------");

  // расчет
  switch (machine_list[Machine].num)
  {

   case 0 : // Перевод: Сферические координаты -> (Theta,Phi)<->(i,j,k)
       E11->Visible=False;
       E22->Visible=False;
       E33->Visible=False;
       switch (Methods)
       {
        case 0 :
          ang[0]=CheckConst(ang[0],180.);
          ang[1]=CheckConst(ang[1],360.);
        break ;
        default : ; break ;
        }
       i=ABCijk_sphera(Methods,ang,ijk);

    OutputMessage(Strmach + "Внимание! В Unigraphics -не общепринятое- обозначение углов !");
    OutputMessage(Strmach + "Внимание!      Угол Phi (UG) -> это угол Theta=0°..+180°");
    OutputMessage(Strmach + "Внимание!      Угол Theta (UG) -> это угол Phi=0°..+360°");
    OutputMessage(Strmach + "Единственное решение.....");

   break;

   case 1 : //MAG3 оси A,C
       switch (Methods)
       {
        case 0 :
          ang[0]=CheckConst(ang[0],360.);
          ang[2]=CheckConst(ang[2],360.);
        break ;
        default : ; break ;
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
   break;

   case 2 : //Variaxis 630-5x оси A(стол),C(стол)
       switch (Methods)
       {
        case 0 :
          ang[0]=CheckConst(ang[0],360.);
          ang[2]=CheckConst(ang[2],360.);
        break ;
        default : ; break ;
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
   break;

   case 3 :    // DMU-50eV оси B(стол),C(стол)
       switch (Methods)
       {
        case 0 :
          ang[1]=CheckConst(ang[1],360.);
          ang[2]=CheckConst(ang[2],360.);
          i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
        break ;
        default :
          if (ijk[2]<0.) { ijk[2]=fabs(ijk[2]);  OutputMessage(Strmach + "Предупреждение! k<0 - ошибка! - исправляем...."); }

          i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);

          mes[0]='\0';sprintf(mes,"Доп. решение 1: B3=%.4lf  С3=%.4lf ",ang[7],ang[8]);
          OutputMessage(Strmach + mes);
          mes[0]='\0';sprintf(mes,"Доп. решение 2: B4=%.4lf  С4=%.4lf ",ang[10],ang[11]);
          OutputMessage(Strmach + mes);
        }
   break;

   case 4 :    // DMU-125PA оси A(голова),C(стол)
       switch (Methods)
       {
        case 0 :
          ang[0]=CheckConst(ang[0],360.);
          ang[2]=CheckConst(ang[2],360.);
        break ;
        default :
          if (EQ_is_zero(ijk[2])) { ijk[2]=1.; OutputMessage(Strmach + "Предупреждение! k=0 - ошибка! - исправляем...."); }
          //if (ijk[2]<0.) { ijk[2]=fabs(ijk[2]);  OutputMessage(Strmach + "Предупреждение! k<0 - ошибка! - исправляем...."); }
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
   break;

   // DMU-125P и DMU-200P оси B(голова),C(стол)
   case 5 : case 6 :
       switch (Methods)
       {
        case 0 :
          ang[1]=CheckConst(ang[1],360.);
          ang[2]=CheckConst(ang[2],360.);
          i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
        break ;
        default :
          //if (EQ_is_zero(ijk[2])) { ijk[2]=1.; OutputMessage(Strmach + "Предупреждение! k=0 - ошибка! - исправляем...."); }
          //if (ijk[2]<0.) { ijk[2]=fabs(ijk[2]);  OutputMessage(Strmach + "Предупреждение! k<0 - ошибка! - исправляем...."); }

          i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);

          //ang[1]=CheckConst(ang[1],360.);
          //ang[2]=CheckConst(ang[2],360.);

          // дополнительное решение
          mes[0]='\0';sprintf(mes,"Доп. решение 1: B3=%.4lf  С3=%.4lf ",ang[7],ang[8]);
          OutputMessage(Strmach + mes);
          mes[0]='\0';sprintf(mes,"Доп. решение 2: B4=%.4lf  С4=%.4lf ",ang[10],ang[11]);
          OutputMessage(Strmach + mes);
        }
   break;

   // V-STAR v3500ORB-оси B,C(голова); // ВФ-5ВС B(=A),C-голова // NC-1325IP оси B(голова),C(голова)// DMU-35M - оси B=-15..+105° , C=±360°
   case 7 : case 8 : case 12 : case 15 :
       switch (Methods)
       {
        case 0 :
          ang[1]=CheckConst(ang[1],360.);
          if (machine_list[Machine].num==7) {
            if (ang[2]>400.) { ang[2]-=360. ; }
            if (ang[2]<-400.) { ang[2]+=360. ; }
          } else {
            ang[2]=CheckConst(ang[2],360.);
          }
          i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
        break ;
        default :
          i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
          if (machine_list[Machine].num==12) {
            if (ang[2]<-200.) ang[2]+=360.;
            if (ang[2]>200.) ang[2]-=360.;
          }
        break ;
        }
   break;

   // FSQV80KR - оси A,B (голова); // MA655c5 ,ФП14V7  - оси A,B (голова)
   case 9 : case 10 : case 11 :
       switch (Methods)
       {
        case 0 :
          ang[0]=CheckConst(ang[0],360.);
          ang[1]=CheckConst(ang[1],360.);
        break ;
        default :
         if (machine_list[Machine].num!=9) {
          if (EQ_is_zero(ijk[2])) { ijk[2]=1.; OutputMessage(Strmach + "Предупреждение! k=0 - ошибка! - исправляем...."); }
          if (ijk[2]<0.) { ijk[2]=fabs(ijk[2]);  OutputMessage(Strmach + "Предупреждение! k<0 - ошибка! - исправляем...."); }
         }
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
        OutputMessage(Strmach + " Преобразование (i,j,k) <-> (A,B) является однозначным. ");
   break;

   case 13 :    // RFP-6K оси A(голова),C(голова)
       switch (Methods)
       {
        case 0 :
          ang[0]=CheckConst(ang[0],360.);
          ang[2]=CheckConst(ang[2],360.);
        break ;
        default :
          if (EQ_is_zero(ijk[2])) { ijk[2]=1.; OutputMessage(Strmach + "Предупреждение! k=0 - ошибка! - исправляем...."); }
          if (ijk[2]<0.) { ijk[2]=fabs(ijk[2]);  OutputMessage(Strmach + "Предупреждение! k<0 - ошибка! - исправляем...."); }
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
   break;

   case 14 :    // АГП-630-800 оси B(стол)
       switch (Methods)
       {
        case 0 :
           ang[1]=CheckConst(ang[1],360.);
        break ;
        default :
          if (!EQ_is_zero(ijk[1])) { OutputMessage(Strmach + "! должно быть j=0 "); }
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
   break;

   case 16 :    // Kлепальный автомат>
       switch (Methods)
       {
        case 0 :
           ang[0]=CheckConst(ang[0],360.);
        break ;
        default :
           if (!EQ_is_zero(ijk[0])) { OutputMessage(Strmach + "! должно быть i=0 "); }
        break ;
        }
        i=ABCijk(machine_list[Machine].methods,Methods,ang,ijk);
   break;

   case 17 : // none
       switch (Methods)
       {
        case 0 :
        break ;
        default : ; break ;
        }
   break;

  }

 // OutputMessage(Strmach + "-----------------------------out-----------------------------");

  if (machine_list[Machine].ai) {
    if (ang[0]<machine_list[Machine].minai || ang[0]>machine_list[Machine].maxai)
    {
      mes[0]='\0'; sprintf(mes,"! %s %.1lf°..%.1lf°",machine_list[Machine].A,machine_list[Machine].minai,machine_list[Machine].maxai);
      OutputMessage(Strmach + mes);
    }
  }
  if (machine_list[Machine].bj) {
    if (ang[1]<machine_list[Machine].minbj || ang[1]>machine_list[Machine].maxbj)
    {
      mes[0]='\0'; sprintf(mes,"! %s %.1lf°..%.1lf°",machine_list[Machine].B,machine_list[Machine].minbj,machine_list[Machine].maxbj);
      OutputMessage(Strmach + mes);
    }
  }
  if (machine_list[Machine].ck) {
    if (ang[2]<machine_list[Machine].minck || ang[2]>machine_list[Machine].maxck)
    {
      mes[0]='\0'; sprintf(mes,"! %s %.1lf°..%.1lf°",machine_list[Machine].C,machine_list[Machine].minck,machine_list[Machine].maxck);
      OutputMessage(Strmach + mes);
    }
  }


/***********************************************************************************************************/

// вывод
  mes[0]='\0'; sprintf(mes,"%.5lf",ang[0]); E1->Text=mes;
  mes[0]='\0'; sprintf(mes,"%.5lf",ang[1]); E2->Text=mes;
  mes[0]='\0'; sprintf(mes,"%.5lf",ang[2]); E3->Text=mes;
  switch (Methods)
  {
   case 0 :  // A,B,C->i,j,k
   break ;
   default : // i,j,k->A,B,C

      mes[0]='\0'; sprintf(mes,"%.5lf",ang[3]); E11->Text=mes;
      mes[0]='\0'; sprintf(mes,"%.5lf",ang[4]); E22->Text=mes;
      mes[0]='\0'; sprintf(mes,"%.5lf",ang[5]); E33->Text=mes;

      // поворотный стол или голова
      if (machine_list[Machine].ck) {
         if (EQ_is_zero(ijk[0]) && EQ_is_zero(ijk[1]) && !EQ_is_zero(ijk[2])) {
          E3->Text="любое"; E33->Text="любое";
        }
      }

  }
  mes[0]='\0'; sprintf(mes,"%.8lf",ijk[0]); E4->Text=mes;
  mes[0]='\0'; sprintf(mes,"%.8lf",ijk[1]); E5->Text=mes;
  mes[0]='\0'; sprintf(mes,"%.8lf",ijk[2]); E6->Text=mes;

 return (0);
}


//---------------------------------------------------------------------------
void __fastcall TFormMain::N1Click(TObject *Sender)
{
  try
  {
    delete ListMachine;
    delete ListDecriptions;
  }
   __finally //(...)
  {
      MessageBeep(0);
   }

  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::N2Click(TObject *Sender)
{
 MessageBeep(0);
 try
  {
    Application->CreateForm(__classid(TFormAbout), &FormAbout);
    FormAbout->ShowModal();
    delete FormAbout;
  }
  catch (Exception &exception)
  {
    ;//Application->ShowException(&exception);
  }
  //OutputMessage("Об авторах..\n\n Авторы программы: коллектив ОГТ-ОПУ, 1961-2005");
  TabControlMachineChange(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCreate(TObject *Sender)
{

  caption=FormMain->Caption;

  int i;
  char s[256], s2[30];
   try
   {

   for (i=0;i<COUNT_MACHINE;i++){
     ListMachine->Add(machine_list[i].name);

     s[0]='\0';sprintf(s,"%s - оси ",machine_list[i].name);
     if (machine_list[i].ai) {
       s2[0]='\0'; sprintf(s2,"%s (%+.1lf,%+.1lf)",machine_list[i].A,machine_list[i].minai,machine_list[i].maxai);
       strcat(s,s2);
     }
     if (machine_list[i].bj) {
       s2[0]='\0'; sprintf(s2,", %s (%+.1lf,%+.1lf)",machine_list[i].B,machine_list[i].minbj,machine_list[i].maxbj);
       strcat(s,s2);
     }
     if (machine_list[i].ck) {
       s2[0]='\0'; sprintf(s2,", %s (%+.1lf,%+.1lf)",machine_list[i].C,machine_list[i].minck,machine_list[i].maxck);
       strcat(s,s2);
     }

     ListDecriptions->Add(s);
   }

   TabControlMachine->Tabs->Assign(ListMachine);
   TabControlMachine->TabIndex=COUNT_MACHINE-1;

   TabControlMachineChange(Sender);
   TabControlMethodsChange(Sender);
   CheckBoxStrClick(Sender);

   E1->Text="0.0000";
   E2->Text="0.0000";
   E3->Text="0.0000";
   E11->Text="";
   E22->Text="";
   E33->Text="";
   Eincr->Text="90";
   E4->Text="0.0000000";
   E5->Text="0.0000000";
   E6->Text="1.0000000";

   }
   __finally //(...)
   {
     ;
    }


}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TabControlMachineChange(TObject *Sender)
{
   int index_mach ;
   index_mach= TabControlMachine->TabIndex ;

   TabControlMachine->Hint = ListMachine->Strings[index_mach];

   StatusBarMes->Font->Color=clRed;
   StatusBarMes->SimpleText= ListDecriptions->Strings[index_mach];

  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  try
  {
    ImageListMachine->GetBitmap(index_mach , Bitmap0);  //  index_mach
    ImageMachine->Picture->Bitmap=Bitmap0 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;

   if (index_mach>=COUNT_MACHINE) return ;

   FormMain->Caption= caption + "  Machine:( " + machine_list[index_mach].name + " )";

   La1->Caption=AnsiString(machine_list[index_mach].A) + "=";
   La2->Caption=AnsiString(machine_list[index_mach].B) + "=";
   La3->Caption=AnsiString(machine_list[index_mach].C) + "=";

   La1->Font->Color=clRed;
   La2->Font->Color=clRed;
   La3->Font->Color=clRed;

   E1->Enabled=False;
   E2->Enabled=False;
   E3->Enabled=False;

   if (machine_list[index_mach].ai) {
     La1->Font->Color=clGreen;
     E1->Enabled=True;
   }
   if (machine_list[index_mach].bj) {
     La2->Font->Color=clGreen;
     E2->Enabled=True;
   }
   if (machine_list[index_mach].ck) {
     La3->Font->Color=clGreen;
     E3->Enabled=True;
   }

}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TabControlMethodsChange(TObject *Sender)
{
   int index_mach,index_meth  ;
   index_mach= TabControlMachine->TabIndex ;
   index_meth= TabControlMethods->TabIndex ;

  Graphics::TBitmap *Bitmap0= new Graphics::TBitmap();
  try
  {
    ImageListButton->GetBitmap(index_meth , Bitmap0);
    BitBtnRun->Glyph= Bitmap0 ;
  }
  catch (...)
  {

  }
  delete Bitmap0 ;

  switch (index_meth)
  {
    case 0:
    E1->ReadOnly=False;
    E2->ReadOnly=False;
    E3->ReadOnly=False;
    E11->Visible=False;
    E22->Visible=False;
    E33->Visible=False;
    E4->ReadOnly=True;
    E5->ReadOnly=True;
    E6->ReadOnly=True;
    //Estr->Text="a,b,c";
    BitBtnFile->Caption="Загрузить УП";
    break ;
    default :
    E1->ReadOnly=True;
    E2->ReadOnly=True;
    E3->ReadOnly=True;
    E11->Visible=True;
    E22->Visible=True;
    E33->Visible=True;
    E4->ReadOnly=False;
    E5->ReadOnly=False;
    E6->ReadOnly=False;
    //Estr->Text="i,j,k";
    BitBtnFile->Caption="Загрузить CLSF";
    break ;
  }

  DecideMachine(index_mach,index_meth);
  return ;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BitBtnRunClick(TObject *Sender)
{
   int index_mach,index_meth  ;
   index_mach= TabControlMachine->TabIndex ;
   index_meth= TabControlMethods->TabIndex ;
   DecideMachine(index_mach,index_meth);
   return ;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::CheckBoxStrClick(TObject *Sender)
{
  Estr->ReadOnly=!(CheckBoxStr->Checked);
  Estr->Enabled=CheckBoxStr->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EstrChange(TObject *Sender)
{
  String estr , pstr ;
  int index;
  double i,j,k;
  double r ;
  char s[256],so[256];
  char word[20];
  int cnt[3];
  double ai,aj,ak;


  int index_mach,index_meth  ;
  index_mach= TabControlMachine->TabIndex ;
  index_meth= TabControlMethods->TabIndex ;

  if (CheckBoxStr->Checked==False) { return ; }

  cnt[0]=-1; cnt[1]=-1; cnt[2]=-1; // счетчики заполнения полей
     i=0.;      j=0. ;     k=0. ;
    ai=0.;     aj=0. ;    ak=0. ;

  /************************************************************/

  estr=""; estr=Estr->Text;
  s[0]='\0';  strcpy(s,estr.c_str());  strcat(s,"\0");

  /************************************************************/

  switch (index_meth)
  {
   case 0 :  // A,B,C->i,j,k

     if (index_mach>=COUNT_MACHINE) return ;

     index=0;
     while (s[index]!='\0')
     {
       if (s[index]=='=' || s[index]==';' || s[index]==',' || \
             s[index]=='(' || s[index]==')' || s[index]=='|' ) s[index]=' ';
       index++;
     }

     if (machine_list[index_mach].ai) {
      if (read_str(s,machine_list[index_mach].A,word))
      {
        if (1==sscanf(word,"%lf",&ai)) {
          /* удаляем строку */ delete_str(s,machine_list[index_mach].A);
          cnt[0]=1;
        }
      }
     }
     if (machine_list[index_mach].bj) {
      if (read_str(s,machine_list[index_mach].B,word))
      {
        if (1==sscanf(word,"%lf",&aj)) {
          /* удаляем строку */ delete_str(s,machine_list[index_mach].B);
          cnt[1]=1;
        }
      }
     }
     if (machine_list[index_mach].ck) {
      if (read_str(s,machine_list[index_mach].C,word))
      {
        if (1==sscanf(word,"%lf",&ak)) {
          /* удаляем строку */ delete_str(s,machine_list[index_mach].C);
          cnt[2]=1;
        }
      }
     }

   break ;
   default : ; // i,j,k->A,B,C
  }

  /************************************************************/

  index=0;
  while (s[index]!='\0')
  {
    if (s[index]=='=' || s[index]==';' || s[index]==',' || \
          s[index]=='(' || s[index]==')' || s[index]=='|' || \
            isalpha(s[index]) ) s[index]=' ';
    index++;
  }

  index=sscanf(s,"%lf %lf %lf",&i,&j,&k);
  
  switch (index_meth)
  {
   case 0 :  // A,B,C->i,j,k

     if (machine_list[index_mach].ai>0 && cnt[0]>0) { i=ai; }
     if (machine_list[index_mach].bj>0 && cnt[1]>0) { j=aj; }
     if (machine_list[index_mach].ck>0 && cnt[2]>0) { k=ak; }

     s[0]='\0'; sprintf(s,"%.5lf",i); E1->Text=s; E1->Update();
     s[0]='\0'; sprintf(s,"%.5lf",j); E2->Text=s; E2->Update();
     s[0]='\0'; sprintf(s,"%.5lf",k); E3->Text=s; E3->Update();

   break ;
   default : // i,j,k->A,B,C

     r=sqrt(i*i+j*j+k*k);
     if (EQ_is_zero(r)) {
       i=0.; j=0.; k=1.; r=1.;
         OutputMessage("Проверка ввода: !! вектор={i,j,k} - нулевой.");
      }
      if (!EQ_is_equal(r,1.000)) {
           s[0]='\0'; sprintf(s,"Проверка ввода: !! вектор={i,j,k} - не нормирован->нормируем... koef=%.4lf ",1./r);
           OutputMessage(s);
           i/= r; j/= r; k/= r;
      }

     s[0]='\0'; sprintf(s,"%.8lf",i); E4->Text=s; E4->Update();
     s[0]='\0'; sprintf(s,"%.8lf",j); E5->Text=s; E5->Update();
     s[0]='\0'; sprintf(s,"%.8lf",k); E6->Text=s; E6->Update();

  }

  DecideMachine(index_mach,index_meth);
  return ;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::BitBtnStrClick(TObject *Sender)
{
   int index_meth  ;
   index_meth= TabControlMethods->TabIndex ;

  switch (index_meth)
  {
    case 0:
    Estr->Text=E1->Text+ " " + E2->Text + " " + E3->Text ;
    break ;
    default :
    Estr->Text=E4->Text+ " " + E5->Text + " " + E6->Text ;
    break ;
  }

  Estr->Update();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::CheckBoxMesFormClick(TObject *Sender)
{
  if (CheckBoxMesForm->Checked==True) { FormMessage->Visible=False; }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtnFileClick(TObject *Sender)
{
  AnsiString filename;
  char info[MAX_PATH + 256];
  long int i ;
  char szFilenam[MAX_PATH+4], szFilename1[MAX_PATH+4];

  int index_mach,index_meth  ;
  index_mach= TabControlMachine->TabIndex ;
  index_meth= TabControlMethods->TabIndex ;

/*  LPTSTR lpCurrentDir; // address of buffer for current directory
  if (0!=GetCurrentDirectory(MAX_PATH+1,lpCurrentDir) ) {
     OpenDialogFile->InitialDir=lpCurrentDir;
  }*/

  OutputMessage(" Данная кнопка предназначена для массового перекодирования UP, CLSF - файлов:");
  OutputMessage("    после перекодировния в папке, содержащей исходный файл:");
  OutputMessage("    будет создан файл с добавление расширения .LOG ");
  OutputMessage("    содержащий строки исходного файла - и расчет углов или i,j,k");

  switch (index_meth)
  {
   case 0 :  // A,B,C->i,j,k
     OpenDialogFile->DefaultExt="UP";
     OpenDialogFile->FileName="";
     OpenDialogFile->Filter = "УП files (*.up)|*.UP|DAT files (*.dat)|*.DAT|MPF files (*.mpf)|*.MPF|PTP files (*.ptp)|*.PTP|Text files (*.txt)|*.TXT|All files (*.*)|*.*";
     OpenDialogFile->Title = "Выбрать UP";
   break ;
   default : // i,j,k->A,B,C
     OpenDialogFile->DefaultExt="CLS";
     OpenDialogFile->FileName="";
     OpenDialogFile->Filter = "CLSF files (*.cls)|*.CLS|All files (*.*)|*.*";
     OpenDialogFile->Title = "Выбрать CLSF";
  }

  if(!OpenDialogFile->Execute()) { return ; }

  filename = OpenDialogFile->FileName;
   //ShowMessage(filename);

   if (0==ParamCount()) {
    ; //ShowMessage(System::CmdLine); //LPTSTR GetCommandLine()
   }

   if (filename=="") {
     ShowMessage("Выберите файл для осуществления расчета....");
     return ;
   }

   if (!FileExists( filename.c_str() )) {
    info[0]='\0';  sprintf(info,"Проверьте наличие Файла ->\n %s \n Расчет прерван....",filename.c_str() );
    OutputMessage(info);
    MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
    return ;
   }

   /* 1 */
   szFilenam[0]='\0'; strcpy( szFilenam,filename.c_str() ); // 1 set the file name
   szFilename1[0]='\0'; strcpy( szFilename1,filename.c_str() ); strcat( szFilename1,".log" ); // 2  set the file name

   if (FileExists(szFilename1)) {
    info[0]='\0';  sprintf(info,"LOG-Файл :\n %s \n \t УЖЕ СУЩЕСТВУЕТ ! \n     Перезаписать....?  ",szFilename1 );
    i=MessageDlg(info, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo,0);
    if (i==mrNo) { OutputMessage("Обработка алгоритма прервана....."); return ; }
   }

  if (index_mach>=COUNT_MACHINE) return ;
  i=machine_list[index_mach].methods;
  if (i==99) i=-1;

  info[0]='\0';
  switch (index_meth)
  {
   case 0 :  i=up_ABCijk(i,0,szFilenam,szFilename1);   break;
   default : i=clsf_ijkABC(i,1,szFilenam,szFilename1); break;
  }
  if (i<0) sprintf(info,"Ошибка работы с файлом.\n Код ошибки=%d \n Расчет прерван....",i);
  else sprintf(info,"Файл  ->\n %s \n создан ... \n Число просмотренных строк =%d ",szFilename1,i);
  OutputMessage(info);
  MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
  if (i<0) return ;

   // notepad
   info[0]='\0';
   strcpy(info,"notepad "); // strcpy(info,"uedit32.exe \"");
   strcat(info, szFilename1 );
   try
   {
     i=WinExec(info,1);
     if (i < 32) {
        info[0]='\0';  sprintf(info, "WinExec failed; error code = %d", i);
        OutputMessage(info);
        //MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
      }
   }
   catch (Exception &exception)
   {
      ; //Application->ShowException(&exception);
   }

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::BitBtnSQRTClick(TObject *Sender)
{
  String estr , pstr ;
  int index;
  double i,j,k;
  double r ;
  char s[80];

  //if (CheckBoxStr->Checked==False) { return ; }

  i=0.; j=0. ; k=0. ;
  estr=""; estr=Estr->Text;
  s[0]='\0';  strcpy(s,estr.c_str());  strcat(s,"\0");
  index=0;
  while (s[index]!='\0')
  {
    if (s[index]==';' || s[index]==',' || s[index]=='(' || s[index]==')' || s[index]=='|') s[index]=' ';
    index++;
  }

  index=sscanf(s,"%lf %lf %lf",&i,&j,&k);

  r=sqrt(i*i+j*j+k*k);

  s[0]='\0'; sprintf(s," = %.10lf",r); Estr->Text=Estr->Text + s; Estr->Update();

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5Click(TObject *Sender)
{
  N5->Checked=!N5->Checked;
  CheckBoxMesForm->Checked=N5->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button1Click(TObject *Sender)
{
  AnsiString str; double var , incr ; char s[80];
  if (E1->Enabled==False) { return ; }
  str=Eincr->Text; incr=atof(str.c_str());
  str=E1->Text; var=atof(str.c_str());
  var+=incr;
  s[0]='\0'; sprintf(s,"%lf",var);   E1->Text=s;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Button2Click(TObject *Sender)
{
  AnsiString str; double var , incr ; char s[80];
  if (E1->Enabled==False) { return ; }
  str=Eincr->Text; incr=atof(str.c_str());
  str=E1->Text; var=atof(str.c_str());
  var-=incr;
  s[0]='\0'; sprintf(s,"%lf",var);   E1->Text=s;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button3Click(TObject *Sender)
{
  AnsiString str; double var , incr ; char s[80];
  if (E2->Enabled==False) { return ; }
  str=Eincr->Text; incr=atof(str.c_str());
  str=E2->Text; var=atof(str.c_str());
  var+=incr;
  s[0]='\0'; sprintf(s,"%lf",var);   E2->Text=s;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button4Click(TObject *Sender)
{
  AnsiString str; double var , incr ; char s[80];
  if (E2->Enabled==False) { return ; }
  str=Eincr->Text; incr=atof(str.c_str());
  str=E2->Text; var=atof(str.c_str());
  var-=incr;
  s[0]='\0'; sprintf(s,"%lf",var);   E2->Text=s;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button5Click(TObject *Sender)
{
  AnsiString str; double var , incr ; char s[80];
  if (E3->Enabled==False) { return ; }
  str=Eincr->Text; incr=atof(str.c_str());
  str=E3->Text; var=atof(str.c_str());
  var+=incr;
  s[0]='\0'; sprintf(s,"%lf",var);   E3->Text=s;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button6Click(TObject *Sender)
{
  AnsiString str; double var , incr ; char s[80];
  if (E3->Enabled==False) { return ; }
  str=Eincr->Text; incr=atof(str.c_str());
  str=E3->Text; var=atof(str.c_str());
  var-=incr;
  s[0]='\0'; sprintf(s,"%lf",var);   E3->Text=s;
}
//---------------------------------------------------------------------------

