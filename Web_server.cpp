#include<stdio.h>
#include<time.h>
 
 
class Web_data_interface {      //trieda  interface
  
int p_data;                     // privatne premenne pocet poloziek v hlavicke
int p_kon;                     // pocet poloziek v body
int p_arch;                            // pocet poloziek v archive
 
public:
struct web_data {               // definicia struktury
       int pocet;               //pocet charov - bytov vo web subore Index
       char header[1100];       //datah lavicky
       char archiv[17000];      //archiv
       char body[13000];        //telo
       char kont_retazec_data[20];    //oddelovaci retazec pre zaciatok archivu
       char kont_retazec_kon[20];     //oddelovaci retazec pre koniec archivu
 
       } w_data;                   //objekt struktury
 
char *ulozena_poz_data[16];  // pozicia pre ukladanie do archivu
char *ulozena_poz_sens[16];
 
web_data *p_struct;                //smernik na strukturu web_data
int get_data();                    //deklaracia funkcii getery, setery
int get_kon();
int get_arch();
 
void set_data(char*,char*);   //seter  uklada oddelovacie retazce do struktury
int hladaj(web_data *);     //funkcia hlada kontrolne retazce v Index pre hlavicku a body, zahadzuje stary archiv
 
};                                 //koniec hlavicky triedy Web_data_interface
 
int Web_data_interface::get_data ()   //getery definicia
 {   return p_data;  }
 
int Web_data_interface::get_kon ()
 {   return p_kon;   }
 
int Web_data_interface::get_arch ()
 {   return p_arch;   }
 
 
void Web_data_interface::set_data(char *retazec_data,char *retazec_kon)  {                  //setter definicia
    for(int i=0;i<12;i++) {
     w_data.kont_retazec_data[i]=*retazec_data++;
     w_data.kont_retazec_kon[i]=*retazec_kon++;
                                     }
}
 
 
int Web_data_interface::hladaj(web_data *p_int)    //funkcia s parametrom smernik na strukturu
{
int pozicia_data =0;                    // lokalne premenne na najdenie pozicie zaciatku oddelovacichs tringov
int pozicia_kon =0;
int pozicia_arch =0;

bool zhoda_data,zhoda_kon;              //boolovske premenne pre logiku hladania oddelovacieho retazca v nacitanych datach 
 
FILE *fp;                               //subor typu file - textovy web file Index
int i=0;
p_int->pocet =0;                     //ukazovatel na strukturu - polozkapocet   - znaky v Index              
 
  fp = fopen("index.html","r");    //otvorenie suboru na citanie               
  if (fp == NULL) {printf("File .... doesn't exist\n"); return (-1);}             //kontrola existencie suboru Index, navrat -1
  else  {
 
 
//testujeme cast header
   for( i=0; i< 12;i++) p_int->header[i] = getc(fp);   //nacitavanie do headeru - prvych 12 znakov - pre porovnavanie s kontrolnym retazcom
   do   {
    zhoda_data = true;
    for(int j=1; j<=12;j++)    if(p_int->header[i-j] != p_int->kont_retazec_data[12-j]) zhoda_data = false;      //pri prvom roznom z 12 false - uz nemoze byt zhoda
 
    if(zhoda_data) pozicia_data= i;                             //ak je zhoda - zapamataj poziciu
         else {
          p_int->header[i] = getc(fp);                          //ak nie je zhoda nacitaj dalsi znak, skontroluj na EOF
          if(p_int->header[i] == EOF) return (0);
          p_int->pocet++;                        // zvyspocet znakov
          i++;
            }
       } while ( zhoda_data == false);  //opakuj, pokial nenajdes zhodu s kontrolnym retazcom pre koniec hlavicky //DATA****
 
//testujeme cast archiv                                      
char temp[17000];            //test priestoru archivu - data teraz nepouzivame - budu sa prepisovat  
//pozicia_arch =12;            // 12 znakov 
for( i=0; i< 12;i++) temp[i]  = getc(fp);            // prvych 12 znakov pre hladanie kon retazca
 do {
    zhoda_kon = true;
    for(int j=1; j<=12;j++)    if( temp[i-j] != p_int->kont_retazec_kon[12-j])  zhoda_kon = false;    //test hladania kon retazca //KON****
    if(zhoda_kon) pozicia_kon= i;                  //ak je zhoda tak ulozit poziciu najdeneho kon  retazca
    temp[i] = getc(fp);   
    pozicia_arch++;                                    //nacitavame archiv - zahodime ho, zistujeme  jeho dlzku
    if(temp[i] == EOF) return (0);                 //ak najdeme eof predcasne v archive - vratime 0 a ukoncime program – v maine
    i++;
    p_int->pocet++;                                     //celkovy pocet znakov v Index           
   } while ( zhoda_kon == false);       //pokial nenajdeme zhodu
   pozicia_arch = pozicia_arch-2; 
 
//testujeme cast body
 for( i=0; i< 12; i++) p_int->body[i] = p_int-> kont_retazec_kon[i];             // v predchadzajucom - najdeny kontrolny retazec ulozim do pola struktury body
 do  {             
  p_int-> body[i] = getc(fp);                    // nacitavame a do body ukladame znaky
  p_int-> pocet++;                                   // v struture incrementujeme pocet
  } while (p_int-> body[i++]!= EOF);     // test na koniec suboru Index  EOF (end of file) 

  pozicia_kon= i-1;                   // zaujima nas pocet znakov v casti bode
  
  } //else
 fclose(fp);                  //zavrieme subor Index na citanie
 
  
p_data=pozicia_data;            // velkost header - pocet charov 
p_kon=pozicia_kon;               //velkost bode - pocet charov
p_arch=pozicia_arch;             //velkost archivu
  
// printf("%d ",p_arch);getchar();//velkost archivu - pocet charov
 
if (pozicia_data*pozicia_kon> 0 ) return 1;  //nasiel oba 1
        else return 0;          //nenasiel oddelovaci string - vrati 0
    
}   //end hladaj
 
 
//*****************class Web_data_file  trieda  s dedenim od Web_data_interface ****************************
 
class Web_data_file: public Web_data_interface  {
        
public:
void save_param(web_data *,int);                //deklaracia funkcie zapis dat
void set_param(char*,char*);                          //deklaracia funkcie na citanie dat
 
char data_[16][8];                    //16 dat pre premenne kazda 8 charov              
char funct_[16][8];                    // nazov 8 charov
};
 
void  Web_data_file::set_param(char *data_akt, char *funct_akt)      //definicia funkcie set_param – bude volana niekde v robotovi
{
char *pfunct; 
char *phodnota;
int i=0;
bool zhoda, ulozene=false;
   
 do {
  ulozene=false;
  pfunct= &funct_[i][0];            //ulozene funkcie
  phodnota= &data_[i][0];             //ulozene data
  
  zhoda = true;                           //inicializacia pre hladanie zhody  pre prazdny retazec
  for (int j=0;j<8;j++)  {if(*(pfunct+j)!= ' ')  zhoda= false;  }           // test na prazdny retazec  - false =  nie je prazdny, staci jeden znak z 12 tich  rôzny
  if (zhoda && (ulozene == false))   for (int k=0;k<8;k++) {*(pfunct+k) = *(funct_akt+k); *(phodnota+k)= *(data_akt+k); ulozene = true;}      // nasiel prvy prazdny !! 
 
  zhoda =true;                        //test na neprazdny retazec
  if (ulozene == false)  for(int j=0;j<8;j++)   if(*(pfunct+j)!= *(funct_akt+j))  zhoda= false; 
  if (zhoda  && (ulozene == false))  for (int k=0;k<8;k++) {*(pfunct+k)= *(funct_akt+k); *(phodnota+k)= *(data_akt+k); ulozene = true; }   // nasiel neprazdny retazec
                                       
  i++;                                         
  if (i==16) ulozene = true;    //maxim 16 velecin
 } while (ulozene == false); 
          
} // end set_param
 
 
void  Web_data_file::save_param(web_data *p_int, int poradie)        //definicia funkcie save_param   - ukladanie dat archivu do struktury – cast archiv na miesto dane parametrom poradie
{
char *point_archiv;      
point_archiv = &p_int->archiv[0];   
  
char p1[]= "sensor_type[";
char p2[]= "] = \"";
char p3[]= "\";\n";
char p4[]= "var data_arch";
char p7[]= "= [";
char p8[]= "];\n";
char p5; char p6;
bool sens_empty;
 
 for (int k=0;k<16;k++)  {                         // pre všetky premenne
 if (poradie == 0)                                       //iba po restarte hlavicku archivu a nulovanie poloziek  !!!!
   { int i;
       switch (k)                                               //pre prevod cisla int do pola charov
      {
         case 0: p5='0';p6='0';  break;      
         case 1: p5='0';p6='1';  break;     
         case 2: p5='0';p6='2';  break;     
         case 3: p5='0';p6='3';  break;     
         case 4: p5='0';p6='4';  break;     
         case 5: p5='0';p6='5';  break;     
         case 6: p5='0';p6='6';  break;     
         case 7: p5='0';p6='7';  break;     
         case 8: p5='0';p6='8';  break;     
         case 9: p5='0';p6='9';  break;     
         case 10: p5='1';p6='0';  break;     
         case 11: p5='1';p6='1';  break;     
         case 12: p5='1';p6='2';  break;     
         case 13: p5='1';p6='3';  break;     
         case 14: p5='1';p6='4';  break;     
         case 15: p5='1';p6='5';  break;     
         default:  p5='0';p6='0';  
        }
  for (i=0;i<12;i++) {*point_archiv = p1[i];  point_archiv++; }   //sensor_type[
  if(k>9)  {*point_archiv= p5; point_archiv++; }              //pre 2 cislice   
  *point_archiv= p6;                            
  point_archiv++;
              
  for(int i=0;i<5;i++) {*point_archiv= p2[i]; point_archiv++; ulozena_poz_sens[k] = point_archiv; }   //] = \"
        
  sens_empty = true;
  for(i=0;i<8;i++) {*point_archiv= funct_[k][i]; 
                    if(*point_archiv != ' ') sens_empty = false;
                    point_archiv++; } // nazov sensoru
  for(int i=0;i<3;i++) {*point_archiv= p3[i]; point_archiv++;  }   //";\n
  for(int i=0;i<13;i++) {*point_archiv= p4[i]; point_archiv++;  }   //var data_arch
   
  if(k>9) {*point_archiv= p5; point_archiv++; *point_archiv= p6;  point_archiv++;}  //pre 2 cislice
        else  {*point_archiv= p6;  point_archiv++;}
        
  for(int i=0;i<3;i++) {*point_archiv= p7[i]; point_archiv++;  }   //= [
   
  for(int i=0;i<8;i++) {
      if(sens_empty == false) *point_archiv= data_[k][i] ;   //data ukladam iba ked nie je prazdny nazov sensora
            else *point_archiv= '0';
      point_archiv++;  }  
  
  *point_archiv=','; point_archiv++;   ulozena_poz_data[k] = point_archiv;  // pozor - ulozit poziciu pre archiv 1. krat
        
  for (int j=1; j<100; j++) {        // pri prvom prechode po restarte napln zvysok archivu nulami 
         for (int i=0;i<8;i++) { *point_archiv= '0'; point_archiv++;  } 
         if (j<99) {*point_archiv=','; point_archiv++;  }      //,     ciarka nie je po poslednom udaji v kazdom poli archivu
             } // for j
  
  for(int i=0;i<3;i++) {*point_archiv= p8[i]; point_archiv++;  }   //];\n  
     
       }  //if  poradie =0
           
  else    //   poradie je >0
    {
     if(poradie >= 100) 
      {      // posun archivu dolava o 1 miesto ak je plna obrazovka
         for(int m=0;m<16;m++) {               
           if(m<=9)  point_archiv = (&p_int->archiv[0])+46 +948*m;    // prve dato   - 948 charov jedna skupuina dat
           if (m>9)  point_archiv = (&p_int->archiv[0])+48+9480 +950*(m-10);
           for(int n=0;n<890;n++) {*point_archiv= *(point_archiv+9); point_archiv++;}     //posun aj vymazanie 0 teho stlpca dat
           ulozena_poz_data[m] =  ulozena_poz_data[m] -9;    //ukladam smernik pre dalsi zapis
                      } //for m
         poradie--;                                 // posuniem dolava a znizim poradie
      }   // if poradie >100
      
      sens_empty = true;         //test prazdneho nazvu  sensora                      
      point_archiv = ulozena_poz_sens[k];      //ukladanie dat do struktury archiv
      for(int i=0;i<8;i++) {*point_archiv= funct_[k][i];
                            if(*point_archiv != ' ') sens_empty = false; 
                            point_archiv++; } // nazov sensoru 
       
      point_archiv = ulozena_poz_data[k];      //ukladanie dat do struktury archiv
      for(int i=0;i<8;i++) {
      if(sens_empty == false) *point_archiv= data_[k][i];   //data ukladam, iba ked nie ja prazdny nazov sensora
      point_archiv++;      }   //i      
      ulozena_poz_data[k] = point_archiv+1;      // plus ciarka je +1
        
    }  // else        poradie  rozne od nuly     
 
 }   //for    k
  
 //for(int i=0;i<15180;i++) printf("%c",p_int->archiv[i]);     getchar();  // velkost archivu 15 180 charov
 
}  // save
 
 
//*****************class Web_data_gener trieda s dedenim od Web_data_file****************************
 
class Web_data_gener: public Web_data_file           //tretia trieda  s dedenim od Web_data_file
{          
public:
 void gener_file(web_data *);    //deklaracia funkcie gener
 void gener_delay(int);    //deklaracia funkcie gener
};
 
void  Web_data_gener::gener_file(web_data *p_int)   //definicia funkcie gener
 
 {
  FILE *fs;
  fs = fopen("index.html","w");                
  int poc_char=Web_data_interface::get_data();       //geterom nacitam pocet znakov v  headeri
  for (int i=0;  i<poc_char; i++)  {   putc(p_int->header[i],fs);     }      //zapis header do suboru
  putc('\n',fs);                            //vlozim do suboru eol 
   
  poc_char=Web_data_interface::get_arch();           //getterom nacitam velkost pola charov archiv !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (int i=0;  i<poc_char; i++) { putc(p_int->archiv[i],fs);  }   //zapis archiv
 
  poc_char=Web_data_interface::get_kon();          //getterom nacitam velkost pola charov body
  for (int i=0;  i<poc_char; i++)  { putc(p_int->body[i],fs);    }          //ulozim data zo struktury cast body do suboru
 
  fclose(fs);                             //zavriet subor na zapis 
 }
 
 
 
void Web_data_gener::gener_delay(int delay)
{
  long int cas1,cas2;
  //time_t cas; 
  cas1 = time(NULL);   //vrati pocet sek od 1.1.1900
  do  
    {cas2 = time(NULL);
   }while (cas2-cas1 <delay ) ; 
  putchar('*');  
}
 
 
//***************main********************************
   int main()                   //main
{ 
char retazec_data[] = "//DATA******"; //oddelovaci retazec na zaciatok archivu
char retazec_kon[] = "//KON*******";   //na koniec archivu
int poradie_arch=0;            // po restarte zacinam plnit archiv od zaciatku
 
 Web_data_gener wd;                    //instancie na triedu Web_data_data
 
 wd.set_data(retazec_data,retazec_kon);           //setter na retazec do struktury
 
 for (int i=0;i<16;i++) for (int j=0;j<8;j++) {wd.data_[i][j]=' '; wd.funct_[i][j]=' '; }      //vynuluj data a nazvy po restarte pre set_param
 
 wd.p_struct = &wd.w_data;            // smernik na strukturu - na objekt w_data
 
int h = wd.hladaj(wd.p_struct);      // ako parameter funkcie hladaj je smernik na w_data
 
 if (h==0) {printf(" nenasiel som retazce \n");  getchar(); return(0); }   //test vystupu z funkcie hladaj - nenajdeny retazec v Index / koniec apky
 if (h==1) printf(" nasiel som retazce \n");     // ak bol return 1 - vsetko ok
 if (h==-1) {printf(" nenasiel som subor Index \n"); getchar(); return(0); }     // nenasiel Index subor / koniec apky
 
//wd.set_param(data_,funct_);     //!!!!!!!!!!!!!! volanie funkcie set_param - TENTO prikaz bude v robotovi 
 
 
do   //nekonecny cyklus
{
 
 
 wd.set_param("81.9    ","tlak31  "); 
 wd.set_param("111.33  ","zapor di"); 
 wd.set_param("5879    ","zrychl  ");
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);                                

 wd.set_param("2445    ","teplota7"); 
 wd.set_param("-5.124  ","tlak31  "); 
 wd.set_param("2421    ","teplota "); 
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);                                

 
 wd.set_param("5874    ","tlak110 "); 
 wd.set_param("58      ","tlak31  "); 

 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);    
 
 wd.set_param("245     ","teplota "); 
 wd.set_param("5.124   ","tlak31  "); 
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);    
 
 wd.set_param("2441    ","teplota7"); 
 wd.set_param("5811    ","tlak31  ");
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);    
 
 wd.set_param("522     ","tlak7   "); 
 wd.set_param("24.44   ","teplota "); 
 wd.set_param("58.788  ","zrychle "); 
 wd.set_param("544     ","tlak31  "); 
 wd.set_param("5111    ","tlak10  "); 
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);    
 
 wd.set_param("240258  ","teplota "); 
 wd.set_param("5.87679 ","zrychle "); 
 wd.set_param("-40258  ","teplota "); 
 wd.set_param("5.879   ","zrych   "); 
 wd.set_param("2445    ","teplota3"); 
 wd.set_param("5.124   ","tlak40  "); 
 wd.set_param("24421   ","teplota1"); 
 wd.set_param("-744    ","tlak110 ");  
 wd.set_param("25.34   ","tlak40  ");                      // len pre testy
 wd.set_param("-0.123  ","dife    "); 
 wd.set_param("5822    ","tlak7   "); 
 wd.set_param("24.44   ","teplota "); 
 wd.set_param("-1.768  ","zrychlen"); 
 wd.set_param("81.9    ","tlak31  "); 
 wd.set_param("111.33  ","zapor di"); 
 wd.set_param("5879    ","zrychl  ");

 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++; 
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);
 
 wd.set_param("2448    ","teplota7"); 
 wd.set_param("-5.124  ","tlak4   "); 
 wd.set_param("2421    ","teplota "); 
 wd.set_param("5874    ","tlak110 "); 
 wd.set_param("58111   ","tlak6   "); 
 wd.set_param("245     ","teplota "); 
 wd.set_param("5.124   ","tlak41  "); 
 wd.set_param("24421   ","teplota6"); 
 wd.set_param("58111   ","tlak10  ");
 wd.set_param("522     ","tlak7   "); 
 wd.set_param("24.44   ","teplota6"); 
 wd.set_param("58.788  ","zrychle "); 
 wd.set_param("-8744   ","tlak11  "); 
 wd.set_param("5111    ","tlak10  "); 
 wd.set_param("240     ","teplota "); 
 wd.set_param("5.87679 ","zrychle "); 
 wd.set_param("-40258  ","teplota "); 
 wd.set_param("5.879   ","zrych   "); 
 wd.set_param("2445    ","teplota3"); 
 wd.set_param("5.124   ","tlak40  "); 
 wd.set_param("24421   ","teplota1"); 
 wd.set_param("-7      ","tlak110 ");  
 wd.set_param("25.34   ","tlak40  ");                      // len pre testy
 wd.set_param("-1.23   ","dife    "); 
 wd.set_param("5822    ","tlak7   "); 
 wd.set_param("12.44   ","teplota "); 
 wd.set_param("58.768  ","zrychlen"); 
 wd.set_param("281.9   ","tlak4   "); 
 wd.set_param("111.33  ","zapor di"); 
 wd.set_param("5879    ","zrychl  ");
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100) poradie_arch ++; 
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);
 
 wd.set_param("2454    ","teplota7"); 
 wd.set_param("-5.124  ","tlak4   "); 
 wd.set_param("2421    ","teplota "); 
 wd.set_param("5874    ","tlak110 "); 
 wd.set_param("-8111   ","tlak6   "); 
 wd.set_param("245     ","teplota "); 
 wd.set_param("5.124   ","tlak41  "); 
 wd.set_param("24421   ","teplota6"); 
 wd.set_param("58111   ","tlak10  ");
 wd.set_param("522     ","tlak7   "); 
 wd.set_param("24.44   ","teplota6"); 
 wd.set_param("580.88  ","zrychle "); 
 wd.set_param("58      ","tlak11  "); 
 wd.set_param("5111    ","tlak10  "); 
 wd.set_param("24      ","teplota "); 
 wd.set_param("5.87679 ","zrychle "); 
 wd.set_param("-40258  ","teplota "); 
 wd.set_param("5.879   ","zrych   "); 
 wd.set_param("25      ","teplota3"); 
 wd.set_param("5.124   ","tlak40  "); 
 wd.set_param("24421   ","teplota1"); 
 wd.set_param("44      ","tlak110 ");  
 wd.set_param("25.34   ","tlak40  ");                      // len pre testy
 wd.set_param("1.23    ","dife    "); 
 wd.set_param("5822    ","tlak7   "); 
 wd.set_param("124.44  ","teplota "); 
 wd.set_param("58.768  ","zrychlen"); 
 wd.set_param("81.9    ","tlak4   "); 
 wd.set_param("111.33  ","zapor di"); 
 wd.set_param("59      ","zrychl  ");
  
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5);   
 
 wd.set_param("2450    ","teplota7"); 
 wd.set_param("-5.124  ","tlak4   "); 
 wd.set_param("2421    ","teplota "); 
 wd.set_param("5874    ","tlak110 "); 
 wd.set_param("58111   ","tlak6   "); 
 wd.set_param("245     ","teplota "); 
 wd.set_param("5.124   ","tlak41  "); 
 wd.set_param("24421   ","teplota6"); 
 wd.set_param("-8111   ","tlak10  ");
 wd.set_param("522     ","tlak7   "); 
 wd.set_param("24.44   ","teplota6"); 
 wd.set_param("-58.78  ","zrychle "); 
 wd.set_param("58744   ","tlak11  "); 
 wd.set_param("5111    ","tlak10  "); 
 wd.set_param("-40258  ","teplota "); 
 wd.set_param("5.87679 ","zrychle "); 
 wd.set_param("-40258  ","teplota "); 
 wd.set_param("5.879   ","zrych   "); 
 wd.set_param("-25     ","teplota3"); 
 wd.set_param("5.124   ","tlak40  "); 
 wd.set_param("-4421   ","teplota1"); 
 wd.set_param("-44     ","tlak110 ");  
 wd.set_param("-25.34  ","tlak4   ");                      // len pre testy
 wd.set_param("-123    ","dife    "); 
 wd.set_param("5822    ","tlak7   "); 
 wd.set_param("-4.44   ","teplota "); 
 wd.set_param("58.768  ","zrychlen"); 
 wd.set_param("81.9    ","tlak31  "); 
 wd.set_param("111.33  ","zapor di"); 
 wd.set_param("5879    ","zrychl  ");

 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5); 
    
 wd.set_param("2448    ","teplota7"); 
 wd.set_param("-5.124  ","tlak4   "); 
 wd.set_param("2421    ","teplota "); 
 wd.set_param("5874    ","tlak110 "); 
 wd.set_param("-8111   ","tlak6   "); 
 wd.set_param("245     ","teplota "); 
 wd.set_param("5.124   ","tlak41  "); 
 wd.set_param("24421   ","teplota6"); 
 wd.set_param("-8111   ","tlak10  ");
 wd.set_param("522     ","tlak7   "); 
 wd.set_param("24.44   ","teplota6"); 
 wd.set_param("58.788  ","zrychle "); 
 wd.set_param("58744   ","tlak11  "); 
 wd.set_param("5111    ","tlak10  "); 
 wd.set_param("240258  ","teplota "); 
 wd.set_param("5.87679 ","zrychle "); 
 wd.set_param("-40258  ","teplota "); 
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100 ) poradie_arch ++;
 wd.gener_file(wd.p_struct);             //volanie funkcie gener - parameter je smernik na strukturu web_data, objekt w_data
 wd.gener_delay(5); 
 
 wd.set_param("5.879   ","zrych   "); 
 wd.set_param("24      ","teplota3"); 
 wd.set_param("5.124   ","tlak40  "); 
 wd.set_param("2449    ","teplota7"); 
 wd.set_param("-4      ","tlak110 ");  
 wd.set_param("25.34   ","tlak4   ");                      // len pre testy
 wd.set_param("-23     ","dife    "); 
 wd.set_param("522     ","tlak7   "); 
 wd.set_param("214.44  ","teplota "); 
 wd.set_param("-8.768  ","zrychlen"); 
 wd.set_param("81.9    ","tlak31  "); 
 wd.set_param("-1.33   ","zapor di"); 
 wd.set_param("59      ","zrychl  ");
 
 wd.save_param(wd.p_struct,poradie_arch);   //volanie funkcie save_param   1x za 5 sekund ulozim do archivu na poziciu poradie_arch
 if (poradie_arch < 100) poradie_arch ++;
 wd.gener_file(wd.p_struct);      
 wd.gener_delay(5); //nekonecny cyklus
 
 
 } while (true);   
 
return(0); 
} //main
