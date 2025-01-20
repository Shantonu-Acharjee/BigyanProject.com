//Coding for IC_TESTER 
//By Bhaskar Jyoti Borah
//Tezpur University, Assam, India: 784028
//For any queries, mail me at : bhaskarjborah5@gmail.com
//SUPPORTED ICs (Manual): 4011, 4030, 4077, 4093, 4071, 4081,4001, 4070, 7266, 7402, 7401, 
//4073, 4075, 4023, 4025, 7411, 5411, 7410, 7412, 7427, 4069, 7404, 7405, 7414, 7400, 74132,
//7403, 7408, 7432, 7486, 5408, 5479, 7409, 5409, 4029, 4000, 74393, 7421, 5421, 7420, 4082, 
//4012, 4002, 4072, 7430, 4068, 4049, 4050, 7474, 7476, 7479,7478,7473
//AUTO-SEARCH SUPPORTED ICs: 4081, 4001, 4070, 4030, 7266, 4077, 4071, 7401, 7402, 4011, 4093
//4073, 4075, 4023, 4025, 7411, 5411, 7410, 7412, 7427, 4029, 74393

#include <lcd.h>
#include <keypad.h>
int good_gate=0,bad_gate=0;
int i=0,x=1,ret=1;
char key;
char str[5];
char str1[5]="---";
char str2[5]="---";
nokia_lcd my(9,10,11,12,13);
matrix_keypad inp(28,26,24,22,23,29,27,25);
void setup()
{ 
  inp.init();
}


//###############################################################################
void loop()
{
  int x=1;
  welcome();
  my.erase();
  my.string("By B.J.Borah",0,'c');
  my.string("I.C. TESTER",2,'c');
  my.string("-----------",3,'c');
  my.string("cont",5,'l');
  my.string("help",5,'r');   
  while(x)
  {
    key=inp.key_in();
    if(key=='r')
    {
      x=0;
      help(); 
    }
    else if(key=='l')
    {
      x=0;
      auto_manual();
    }
  }
}


//###############################################################################
void auto_manual()
{
  int count=1;
  int y=1;
  int key;
  my.erase();
  my.string("*Auto-search IC",1,'l');
  my.string("*Manual mode",2,'l');
  my.string("(use up/down)",4,'c');
  my.string("ok",5,'c');
  my.string("<<",count,'r');
  while(y)
  {
    key=inp.key_in();
    if(key=='u')
    {
      if(count==2)
      {
        my.erase_row(count);
        my.string("*Manual mode",2,'l');
        count--;
        my.string("<<",count,'r');
      }
    }
    else if(key=='d')
    {
      if(count==1)
      {
        my.erase_row(count);
        my.string("*Auto-search",1,'l');
        count++;
        my.string("<<",count,'r');
      }
    }
    else if(key=='m')
    {
      //y=0;
      if(count==1)
        y=auto_search();
      else if(count==2)
        y=ic_test();
    }
  }
}


//###############################################################################

int auto_search()
{
  int res;
  int i,j,k,in1,in2,in3,out,temp=0,is_ok4029ic=0,ind=0,tem=0,dec_out=0,is_ok74393c1=0,is_ok74393c2=0,prev1,prev2,out1,out2,first=1,state;
  int arr[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int arr1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int arr2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  int x;
  int key;
  int p[16]={34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
  int s[8]={0,0,0,0,0,0,0,0};
  int q[4]={0,0,0,0};
  int r[4]={0,0,0,0};
  int tri[3]={0,0,0};
  int trip[3]={0,0,0};
  my.erase();
  my.string("Auto-search",0,'c');
  my.string("-----------",1,'c');
  my.string("No. of pins:",2,'c');
  res=accpt('z');
  if(res==0)
    return 0;
  else if(res>16)
  {
    my.erase();
    my.string("Beyond limit!",1,'c');
    my.string("------------",2,'c');
    my.string("try again!",4,'c');
    delay(600);
    auto_manual();
  }
  else if(res==14)  
  {
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
          pinMode(p[i],OUTPUT);
          break;
        case 4:
        case 6:
        case 7:
        case 9:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    while(x<=4)
    {
      switch(x)
      {
        case 1:
          in1=0,in2=2,out=4;
          break;
        case 2:
          in1=10,in2=8,out=6;
          break;
        case 3:
          in1=13,in2=11,out=9;
          break;
        case 4:
          in1=3,in2=5,out=7;
          break;
      }
      for(i=0;i<2;i++)
      {
         for(j=0;j<2;j++)
         {
           digitalWrite(p[in1],i);
           digitalWrite(p[in2],j);
           delay(0.5);
           s[temp]=digitalRead(p[out]);
           temp++;
         }
       }
       if(s[0]!=0 && s[1]!=0 && s[2]!=0 && s[3]==0)
         q[x-1]=1; //4011,4093---NAND
       else if(s[0]==0 && s[1]!=0 && s[2]!=0 && s[3]!=0)
         q[x-1]=6; //4071---OR
       else if(s[0]==0 && s[1]==0 && s[2]==0 && s[3]!=0)
         q[x-1]=2; //4081---AND
       else if(s[0]!=0 && s[1]==0 && s[2]==0 && s[3]==0)
         q[x-1]=3;  //4001---NOR
       else if(s[0]==0 && s[1]!=0 && s[2]!=0 && s[3]==0)
         q[x-1]=4; //4070,4030---EX-OR
       else if(s[0]!=0 && s[1]==0 && s[2]==0 && s[3]!=0)
         q[x-1]=5; //7266,4077---EX-NOR
       
         
       x=x+1;
       temp=0;
    }

    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 1:
        case 2:
        case 4:
        case 5:
        case 7:
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
          pinMode(p[i],OUTPUT);
          break;
        case 0:
        case 3:
        case 6:
        case 9:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    temp=0;
    while(x<=4)
    {
      switch(x)
      {
        case 1:
          in1=4,in2=2,out=0;
          break;
        case 2:
          in1=10,in2=8,out=6;
          break;
        case 3:
          in1=13,in2=11,out=9;
          break;
        case 4:
          in1=7,in2=5,out=3;
          break;
      }
      for(i=0;i<2;i++)
      {
         for(j=0;j<2;j++)
         {
           digitalWrite(p[in1],i);
           digitalWrite(p[in2],j);
           delay(1);
           s[temp]=digitalRead(p[out]);
           temp++;
         }
       }
       if(s[0]!=0 && s[1]!=0 && s[2]!=0 && s[3]==0)
         r[x-1]=7; //7401---NAND
       else if(s[0]!=0 && s[1]==0 && s[2]==0 && s[3]==0)
         r[x-1]=8;  //7402---NOR
    
       x=x+1;
       temp=0;
    }
    
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 11:
        case 12:
          pinMode(p[i],OUTPUT);
          break;
        case 5:
        case 10:
        case 13:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    temp=0;
    while(x<=3)
    {
      switch(x)
      {
        case 1:
          in1=0,in2=2,in3=3,out=5;
          break;
        case 2:
          in1=4,in2=6,in3=8,out=10;
          break;
        case 3:
          in1=7,in2=9,in3=11,out=13;
          break; 
      }
      for(i=0;i<2;i++)
      {
        for(j=0;j<2;j++)
        {
          for(k=0;k<2;k++)
          {
            digitalWrite(p[in1],i);
            digitalWrite(p[in2],j);
            digitalWrite(p[in3],k);
            delay(1);
            s[temp]=digitalRead(p[out]);
            temp++;
          }
        }
      }
      
         
      if(s[0]==0 && s[1]==0 && s[2]==0 && s[3]==0 && s[4]==0 && s[5]==0 && s[6]==0 && s[7]!=0)
        tri[x-1]=11; //7411, 5411---AND
         
      else if(s[0]!=0 && s[1]!=0 && s[2]!=0 && s[3]!=0 && s[4]!=0 && s[5]!=0 && s[6]!=0 && s[7]==0)
        tri[x-1]=12; //7410, 7412---NAND
      
      else if(s[0]!=0 && s[1]==0 && s[2]==0 && s[3]==0 && s[4]==0 && s[5]==0 && s[6]==0 && s[7]==0)
        tri[x-1]=13; //7427---NOR

       x=x+1;
       temp=0;
    }
    
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 12:
        case 13:
          pinMode(p[i],OUTPUT);
          break;
        case 10:
        case 9:
        case 11:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    temp=0;
    while(x<=3)
    {
      switch(x)
      {
        case 1:
          in1=0,in2=2,in3=13,out=11;
          break;
        case 2:
          in1=4,in2=6,in3=8,out=10;
          break;
        case 3:
          in1=3,in2=5,in3=7,out=9;
          break; 
      }
      for(i=0;i<2;i++)
      {
        for(j=0;j<2;j++)
        {
          for(k=0;k<2;k++)
          {
            digitalWrite(p[in1],i);
            digitalWrite(p[in2],j);
            digitalWrite(p[in3],k);
            delay(1);
            s[temp]=digitalRead(p[out]);
            temp++;
          }
        }
      }
   
         
      if(s[0]!=0 && s[1]!=0 && s[2]!=0 && s[3]!=0 && s[4]!=0 && s[5]!=0 && s[6]!=0 && s[7]==0)
        trip[x-1]=14; //4023---NAND
      
      else if(s[0]!=0 && s[1]==0 && s[2]==0 && s[3]==0 && s[4]==0 && s[5]==0 && s[6]==0 && s[7]==0)
        trip[x-1]=15; //4025---NOR
        
      else if(s[0]==0 && s[1]==0 && s[2]==0 && s[3]==0 && s[4]==0 && s[5]==0 && s[6]==0 && s[7]!=0)
        trip[x-1]=9; //4073---AND
         
      else if(s[0]==0 && s[1]!=0 && s[2]!=0 && s[3]!=0 && s[4]!=0 && s[5]!=0 && s[6]!=0 && s[7]!=0)
        trip[x-1]=10;  //4075---OR
      
       
       x=x+1;
       temp=0;
    }

    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 12:
          pinMode(p[i],OUTPUT);
          break;
        case 4:
        case 6:
        case 8:
        case 10:
        case 7:
        case 9:
        case 11:
        case 13:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 

    digitalWrite(p[3],0);
    digitalWrite(p[0],0);

    digitalWrite(p[2],1);
    digitalWrite(p[5],1);
    delay(5);
    digitalWrite(p[2],0);
    digitalWrite(p[5],0);

    for(i=0;i<=15;i++)
    {
      delay(2);
      digitalWrite(p[0],0); //clock1
      delay(2);
      digitalWrite(p[3],0); //clock2
      
      state=digitalRead(p[10]);
        out1=state*8;
      state=digitalRead(p[8]);
        out1=out1+(state*4);
      state=digitalRead(p[6]);
        out1=out1+(state*2);
      state=digitalRead(p[4]);
        out1=out1+state;  
      arr1[i]=out1;
            
      state=digitalRead(p[13]);
        out2=(state*8);
      state=digitalRead(p[11]);
        out2=out2+(state*4);
      state=digitalRead(p[9]);
        out2=out2+(state*2);
      state=digitalRead(p[7]);
        out2=out2+state;
      arr2[i]=out2;

      digitalWrite(p[0],1);
      delay(1);
      digitalWrite(p[3],1);
    }
    for(i=1;i<15;i++)
    {
      if((arr1[i+1]-arr1[i])==1)
      {
        is_ok74393c1=1;
      }
      else
      {
        is_ok74393c1=0;
        break;
      }
      if((arr2[i+1]-arr2[i])==1)
      {
        is_ok74393c2=1;
      }
      else
      {
        is_ok74393c2=0;
        break;
      }
    }
    
    my.erase();
    my.string("RESULT",0,'c');
    my.string("------",1,'c');
    my.string("exit",5,'r');

    if((is_ok74393c1==1) && (is_ok74393c2==1))
    {
      my.string("IC is:",2,'c');
      my.string("74393",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    
    else if(q[0]==2 && q[1]==2 && q[2]==2 && q[3]==2)
    {
      my.string("IC is:",2,'c');
      my.string("4081",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(q[0]==3 && q[1]==3 && q[2]==3 && q[3]==3)
    {
      my.string("IC is:",2,'c');
      my.string("4001",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(q[0]==4 && q[1]==4 && q[2]==4 && q[3]==4)
    {
      my.string("IC is:",2,'c');
      my.string("4070/4030",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(q[0]==5 && q[1]==5 && q[2]==5 && q[3]==5)
    {
      my.string("IC is:",2,'c');
      my.string("7266/4077",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(q[0]==6 && q[1]==6 && q[2]==6 && q[3]==6)
    {
      my.string("IC is:",2,'c');
      my.string("4071",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(r[0]==7 && r[1]==7 && r[2]==7 && r[3]==7)
    {
      my.string("IC is:",2,'c');
      my.string("7401",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(r[0]==8 && r[1]==8 && r[2]==8 && r[3]==8)
    {
      my.string("IC is:",2,'c');
      my.string("7402",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(q[0]==1 && q[1]==1 && q[2]==1 && q[3]==1)
    {
      my.string("IC is:",2,'c');
      my.string("4011/4093",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(trip[0]==9 && trip[1]==9 && trip[2]==9)
    {
      my.string("IC is:",2,'c');
      my.string("4073",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(trip[0]==10 && trip[1]==10 && trip[2]==10)
    {
      my.string("IC is:",2,'c');
      my.string("4075",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(trip[0]==14 && trip[1]==14 && trip[2]==14)
    {
      my.string("IC is:",2,'c');
      my.string("4023",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(trip[0]==15 && trip[1]==15 && trip[2]==15)
    {
      my.string("IC is:",2,'c');
      my.string("4025",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(tri[0]==11 && tri[1]==11 && tri[2]==11)
    {
      my.string("IC is:",2,'c');
      my.string("7411/5411",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(tri[0]==12 && tri[1]==12 && tri[2]==12)
    {
      my.string("IC is:",2,'c');
      my.string("7410/7412",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else if(tri[0]==13 && tri[1]==13 && tri[2]==13)
    {
      my.string("IC is:",2,'c');
      my.string("7427",3,'c');
      my.string("IC is GOOD",4,'c');
    }
    else
      my.string("Not detected!",2,'c');
   
    while(1)
    {
      delay(5);
      key=inp.key_in();
      if(key=='r')
      {
        return 0;
      }
    }
  }

  else if(res==16)
  {
      for(i=0;i<16;i++)
      {
        switch(i)
        {  
        case 0:
        case 1:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 13:
        case 14:
        case 15:
          pinMode(p[i],OUTPUT);
          break;
        case 2:
        case 5:
        case 10:
        case 11:
        case 12:
          pinMode(p[i],INPUT);
          break;
        }
      }
      digitalWrite(p[14],0);
      digitalWrite(p[1],1); 
      
      digitalWrite(p[6],0);
      digitalWrite(p[9],0);
      digitalWrite(p[7],0);
      digitalWrite(p[4],0);
      
      digitalWrite(p[13],1);
      digitalWrite(p[0],1);
      delay(5);
      digitalWrite(p[0],0);
      digitalWrite(p[15],1);
      
      temp=1;
      ind=0;
      while(temp)
      {
        digitalWrite(p[3],0);
        tem=digitalRead(p[2]);
          dec_out=tem*8;
        tem=digitalRead(p[5]);
          dec_out=dec_out+tem*4;
        tem=digitalRead(p[11]);
          dec_out=dec_out+tem*2;
        tem=digitalRead(p[10]);
          dec_out=dec_out+tem;
         
        arr[ind]=dec_out;
        ind++;
        digitalWrite(p[3],1);
        delay(5);
        if(digitalRead(p[12])==0)
        {
          temp=0;
        } 
      }
      ind=0;
      //temp=1;
      while(ind<=14)
      {
        if(arr[ind]==ind)
        {
          is_ok4029ic=1; 
        }
        else
        {
          is_ok4029ic=0;
          break;
        }
        ind++;
      }
      my.erase();
      my.string("RESULT",0,'c');
      my.string("------",1,'c');
      my.string("exit",5,'r');
      if(is_ok4029ic==1)
      {
        my.string("IC is:",2,'c');
        my.string("4029",3,'c');
        my.string("IC is GOOD",4,'c');
      }
      else
      {
        my.string("Not detected!",2,'c');
      }
      while(1)
      {
        delay(5);
        key=inp.key_in();
        if(key=='r')
        {
         return 0;
        }
      }
  }
}


//###############################################################################
int welcome()
{
  my.erase();
  my.string("skip",5,'r');
  my.string("W E L C O M E",0,'c');
  my.string("-------------",1,'c');
  for(i=0;i<100;i++)
  {
    delay(2);
    key=inp.key_in();
    if(key=='r')
      return 0;
  }
  my.string("By",2,'c');
  for(i=0;i<100;i++)
  {
    delay(2);
    key=inp.key_in();
    if(key=='r')
      return 0;
  }
  my.string("--B.J.Borah--",3,'c');
  for(i=0;i<100;i++)
  {
    delay(2);
    key=inp.key_in();
    if(key=='r')
      return 0;
  }
  my.string("===TEZU===",4,'c');
  for(i=0;i<400;i++)
  {
    delay(6);
    key=inp.key_in();
    if(key=='r')
      return 0;
  }
}



//###############################################################################
int help()
{
  int temp;  
  go1:
  my.erase();
  my.string("HELP",0,'c');
  my.string("*IC in Socket",1,'l');
  my.string("*Enter IC no.",2,'l');
  my.string("*Press OK",3,'l');
  my.string("*Wait 4 Result",4,'l');
  my.string("nxt",5,'c');
  my.string("ext",5,'l');
  temp=1;
  while(temp)
  {
    delay(0.5);
    key=inp.key_in();
    if(key=='l')
    {
      my.erase();
        return 0;
    }
    else if(key=='m')
      temp=0;
  }
  go2:
  my.erase();
  my.string("HELP",0,'c');
  my.string("*Proper ic no.",1,'l');
  my.string("(avoid dmage)",2,'l');
  my.string("*Dont use when",3,'l');
  my.string(" -no IC added",4,'l');
  my.string("nxt",5,'c');
  my.string("ext",5,'l');
  my.string("bck",5,'r');
  temp=1;
  while(temp)
  {
    delay(0.5);
    key=inp.key_in();
    if(key=='l')
    {
      my.erase();
      return 0;
    }
    else if(key=='m')
      temp=0;
    else if(key=='r')
      goto go1;
  }
  go3:
  my.erase();
  my.string("HELP",0,'c');
  my.string("*IC from LEFT",1,'l');
  my.string("*Use proprly!",2,'l');
  my.string("==GOOD LUCK==",4,'c');
  my.string("ext",5,'l');
  my.string("bck",5,'r');
  temp=1;
  while(temp)
  {    
    delay(0.5);
    key=inp.key_in();
    if(key=='l')
    {
      my.erase();
      return 0;
    }
    else if(key=='r')
      goto go2;
  }
}


//###############################################################################
int ic_test()
{
  long int num;
  int x=1;
  while(x)
  {
    num=accpt('i');
    if(num==0)
      return 0;
    switch(num)
    {
      case 0:
        x=0;
        break;
      case 4011:
      case 4030:
      case 4077:
      case 4093:
      case 4071:
      case 4081:
      case 4001:
      case 4070:
      case 7266:
      case 7402:
      case 7401:
        x=ic_quad_2(num);
        break;
      case 4073:
      case 4075:
      case 4023:
      case 4025:
      case 7411:
      case 5411:
      case 7410:
      case 7412:
      case 7427:
        x=ic_triple_3(num); 
        break;
      case 4069:
      case 7404:
      case 7405:
      case 7414:
        x=ic_hex_inv(num);
        break;
      case 7400:
      case 74132:
      case 7403:
      case 7408:
      case 7432:
      case 7486:
      case 5408:
      case 7409:
      case 5409:
        x=ic_74_quad_2(num);
        break;
      case 4029:
        x=ic_4029();
        break; 
      case 4000:
        x=ic_4000();
        break;
      case 74393:
        x=ic_74393();
        break; 
      case 7421:
      case 5421:
      case 7420:
      case 4082:
      case 4012:
      case 4002:
      case 4072:
        x=ic_dual_4(num);
        break;
      case 7430:
      case 4068:
        x=ic_oct_1(num);
        break;
      case 4049:
      case 4050:
        x=ic_hex_16pin(num);
        break;
      case 7474:
      case 7479:
      case 5479:
        x=ic_d_flip(num);
        break;
      case 7476:
      case 7473:
        x=ic_jk_flip(num);
        break;
      default: 
        my.erase();
        my.string("I.C. no.",0,'c');
        my.string("--------",1,'c');
        my.dis_num(num,3,'c');
        my.string("NOT INCLUDED",5,'c');
        delay(1000);
        break;
    }
  }
}



//###############################################################################
long int accpt(char ch)
{
  go2:
  if(ch!='z')
    my.erase();
  if(ch=='b')
  {
    my.string("Input Binary",0,'c');
    my.string("------------",1,'c');
  }
  else if(ch=='d')
  {
    my.string("Input Decimal",0,'c');
    my.string("-------------",1,'c');
  }
  else if(ch=='i')
  {
    my.string("Enter IC no.",0,'c');
    my.string("------------",1,'c');
  }
  else if(ch=='c')
  {
    my.string("Time interval",0,'c');
    my.string("-------------",1,'c');
    my.string("(in milisec)",4,'c');
  }
  my.string("ok",5,'c');
  my.string("clr",5,'r');
  if(ch=='b' || ch=='d' || ch=='c')
    my.string("bck",5,'l');
  else
    my.string("ext",5,'l');
  unsigned int d=100,p=1;
  long int num=0;
  while(p)
  {  
    key=inp.key_in();
    switch(key)
    {
      case '1':
        d=1;
        break;
      case '2':
        d=2;
        break;
      case '3':
        d=3;
        break;
      case '4':
        d=4;
        break;
      case '5':
        d=5;
        break;
      case '6':
        d=6;
        break;
      case '7':
        d=7;
        break;
      case '8':
        d=8;
        break;
      case '9':
        d=9;
        break;
      case '0':
        d=0;
        break;
      case 'm':
        d=10;
        break;
      case 'r':
        d=11;
        break;
      case 'l':
        d=12;
        break;
    }
    if(d<10)
    {
      if(num<=9999)
        num=num*10+d;
      if(ch=='b')
      {
        if(d!=0 && d!=1)
        {
          my.erase();
          my.string("ONLY 0 & 1",1,'c');
          my.string("----------",2,'c');
          my.string("..TRY AGAIN..",4,'c');
          delay(800);                                                          
          goto go2;
        }
      }
      if(ch=='d')
      {
        if(num>15)
        {
          my.erase();
          my.string("MAX UPTO 15",1,'c');
          my.string("-----------",2,'c');
          my.string("..TRY AGAIN..",4,'c');
          delay(1700);
          goto go2;
        }
      }
      my.erase_row(3);
      my.dis_num(num,3,'c');
      d=100; 
    }
    else if(d==10)
    {
      p=0;
    }
    else if(d==11)
    {
      num=(num/10); 
      my.erase_row(3);
      my.dis_num(num,3,'c');
      d=100;
    }
    else if(d==12)
    {
      x=0;
      ret=0;
      return 0;
    }  
  }
  if(num==0)
  {
    if(ch!='b' && ch!='d')
    {
      my.erase();
      my.string("TRY AGAIN",2,'c');
      my.string("---------",3,'c');
      delay(500);
      my.erase_row(3);
      if(ch=='z')
      {
        my.string("Auto-search",0,'c');
        my.string("-----------",1,'c');
        my.string("No. of pins:",2,'c');
      }
      goto go2;
    }
  }
  return (num);
}
// IC testing functions
//###############################################################################
//I.C. quad-2-input....
int ic_quad_2(int num)
{
  bhaskar:
  int i,j,detl=0,x=1;
  int count;
  int in1,in2,out,state,is_ok;
  my.erase();
  my.string("PLACE THE IC",0,'c');
  my.dis_num(num,1,'c'); 
  if(num==4011 || num==7401 || num==4093)
  {
    my.string("QUAD-2-INPUT",3,'c');
    my.string("..NAND GATE..",4,'c');
  }
  if(num==7266 || num==4077)
  {
    my.string("QUAD-2-INPUT",3,'c');
    my.string("Exclusive NOR",4,'c');
  }
  if(num==4070 || num==4030)
  {
    my.string("QUAD-2-INPUT",3,'c');
    my.string("Exclusive OR",4,'c');
  }
  if(num==4001 || num==7402)
  {
    my.string("QUAD-2-INPUT",3,'c');
    my.string("..NOR GATE..",4,'c');
  }
  else if(num==4071)
  {
    my.string("QUAD-2-INPUT",3,'c');
    my.string("..OR GATE..",4,'c');
  } 
  else if(num==4081)
  {
    my.string("QUAD-2-INPUT",3,'c');
    my.string("..AND GATE..",4,'c');
  }
  my.string("ext",5,'l');
  my.string("chk",5,'c');
  my.string("bck",5,'r');
  while(x)
  {
    delay(5);
    key=inp.key_in();
    if(key=='l')
    {
      return 0;
    }
    else if(key=='r')
    {
      return 1;
    }
    else if(key=='m')
    {
      x=0;
      int y=1;
      my.erase();
      my.string("TRUTH TABLE",1,'c');
      my.string("-----------",2,'c');
      my.string("????",3,'c');
      my.string("yes",5,'c');
      my.string("no",5,'l');
      my.string("bck",5,'r');
      while(y)
      {
        key=inp.key_in();
        if(key=='m')
        {
          detl=1;
          y=0;
        }
        else if(key=='l')
        {
          detl=0;
          y=0;
        }
        else if(key=='r')
        {
          goto bhaskar;
        } 
      }
    }
  }
  x=1;
  int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
  if(num==7402 || num==7401)
  {
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 1:
        case 2:
        case 4:
        case 5:
        case 7:
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
          pinMode(p[i],OUTPUT);
          break;
        case 0:
        case 3:
        case 6:
        case 9:
          pinMode(p[i],INPUT);
          break;
      }
    }
  }
  else
  {
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
          pinMode(p[i],OUTPUT);
          break;
        case 4:
        case 6:
        case 7:
        case 9:
          pinMode(p[i],INPUT);
          break;
      }
    }
  }
  digitalWrite(p[12],0);
  digitalWrite(p[1],1); 
  x=1;
  while(x<=4)
  {
    is_ok=1;
    if(num==7402 || num==7401)
    {
      switch(x)
      {
        case 1:
          in1=4,in2=2,out=0;
          break;
        case 2:
          in1=10,in2=8,out=6;
          break;
        case 3:
          in1=13,in2=11,out=9;
          break;
        case 4:
          in1=7,in2=5,out=3;
          break;
      }
    }
    else
    { 
      switch(x)
      {
        case 1:
          in1=0,in2=2,out=4;
          break;
        case 2:
          in1=10,in2=8,out=6;
          break;
        case 3:
          in1=13,in2=11,out=9;
          break;
        case 4:
          in1=3,in2=5,out=7;
          break;
      }
    }
    my.erase();
    count=1;
    for(i=0;i<2;i++)
    {
      for(j=0;j<2;j++)
      {
        digitalWrite(p[in1],i);
        digitalWrite(p[in2],j);
        delay(1);
        state=digitalRead(p[out]);
        delay(1);
        if(detl==1)
        {
          my.string("IN",0,'l');
          my.string("IN",0,'c');
          my.string("OUT",0,'r');
          my.string("skp",5,'r');
          my.string("pse",5,'l');
          my.dis_num(i,count,'l');
          my.dis_num(j,count,'c');
          my.dis_num(state,count,'r');
        }
        if(num==4011 || num==7401 || num==4093)
        {
          if(i==0 && j==0 && state==0)
            is_ok=0;
          else if(i==0 && j==1 && state==0)
            is_ok=0;
          else if(i==1 && j==0 && state==0)
            is_ok=0;
          else if(i==1 && j==1 && state!=0)
            is_ok=0;
        }
        else if(num==4081)
        { 
          if(i==0 && j==0 && state!=0)
            is_ok=0;
          else if(i==0 && j==1 && state!=0)
            is_ok=0;
          else if(i==1 && j==0 && state!=0)
            is_ok=0;
          else if(i==1 && j==1 && state==0)
            is_ok=0; 
        }
        else if(num==4071)
        {
          if(i==0 && j==0 && state!=0)
            is_ok=0;
          else if(i==0 && j==1 && state==0)
            is_ok=0;
          else if(i==1 && j==0 && state==0)
            is_ok=0;
          else if(i==1 && j==1 && state==0)
            is_ok=0;
        } 
        else if(num==4001 || num==7402)
        {
          if(i==0 && j==0 && state==0)
            is_ok=0;
          else if(i==0 && j==1 && state!=0)
            is_ok=0;
          else if(i==1 && j==0 && state!=0)
            is_ok=0;
          else if(i==1 && j==1 && state!=0)
            is_ok=0;
        } 
        else if(num==4070 || num==4030)
        {
          if(i==0 && j==0 && state!=0)
            is_ok=0;
          else if(i==0 && j==1 && state==0)
            is_ok=0;
          else if(i==1 && j==0 && state==0)
            is_ok=0;
          else if(i==1 && j==1 && state!=0)
            is_ok=0;
        } 
        else if(num==7266 || num==4077)
        {
          if(i==0 && j==0 && state==0)
            is_ok=0;
          else if(i==0 && j==1 && state!=0)
            is_ok=0;
          else if(i==1 && j==0 && state!=0)
            is_ok=0;
          else if(i==1 && j==1 && state==0)
            is_ok=0;
        }
        count++;
      }
    }
    int temp1=0,temp2=0;
    if(detl==1)
    { 
      while(temp1<350)
      {
        delay(9);
        key=inp.key_in();
        if(key=='r')
          temp1=350;
        else if(key=='l')
        {
          my.erase_row(5);
          my.string("cont",5,'c');
          while(temp2<350)
          {
            delay(7);
            key=inp.key_in();
            if(key=='m')
            {
              temp2=350;
              temp1=350;
            }
          }
        }
        temp1++;
      }
    }
    my.erase();
    delay(2);  
    my.string("GATE",0,'c');
    my.string("skp",5,'r');
    my.dis_num(x,2,'c');
    if(is_ok==1)
    {
      my.string("OK",4,'c');
      good_gate++;
    }
    else if(is_ok==0)
    {
      my.string("NOT OK",4,'c');
      bad_gate++;
    }
    temp1=0;
    while(temp1<300)
    {
      delay(8);
      key=inp.key_in();
      if(key=='r')
      {
        temp1=300;
      }
      temp1++;
    }
    x++;
  }
  result_gate(num);
} 
//###############################################################################
int result_gate(long int x)
{
    my.erase();
    my.string("RESULT",0,'c');
    my.string("------",1,'c');
    my.string("exit",5,'r');
    if(good_gate>1)
    {
      my.dis_num(good_gate,2,'l');
      my.string(" gates GOOD",2,'c');
    }
    else if(good_gate==1)
    {
      my.dis_num(good_gate,2,'l');
      my.string(" gate GOOD",2,'c');
    }
    else if(good_gate==0)
    {
      my.string("NO GOOD GATE",2,'l');
    }
    if(bad_gate>1)
    {
      my.dis_num(bad_gate,3,'l');
      my.string(" gates BAD",3,'c');
    }
    else if(bad_gate==1)
    {
      my.dis_num(bad_gate,3,'l');
      my.string(" gate BAD",3,'c');
    }
    else if(bad_gate==0)
    {
      my.string("NO BAD GATE",3,'l');
    }
    my.string("I.C.",4,'l');
    my.dis_num(x,4,'c');
    if(bad_gate==0)
      my.string("GOOD",4,'r');
    else
      my.string("BAD",4,'r');
      
    bad_gate=0;
    good_gate=0;
    while(1)
    {
      delay(7);
      key=inp.key_in();
      if(key=='r')
      {
          return 0;
      }
    } 
}
//IC triple-3-input....
//###############################################################################
int ic_triple_3(int num)
{
    bhaskar:
    int i,j,k,detl=0,x=1;
    int temp1=0,temp2=0;
    int count,c,t;
    int in1,in2,in3,out,state,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
      
    if(num==4073)
    {
      my.string("TRIPLE-3-IN",3,'c');
      my.string("..AND GATE..",4,'c');
    }
    if(num==4075)
    {
      my.string("TRIPLE-3-IN",3,'c');
      my.string("..OR GATE..",4,'c');
    }
    if(num==7411 || num==5411)
    {
      my.string("TRIPLE-3-IN",3,'c');
      my.string("..AND GATE..",4,'c');
    }
    if(num==7410 || num==7412 || num==4023)
    {
      my.string("TRIPLE-3-IN",3,'c');
      my.string("..NAND GATE..",4,'c');
    }
    if(num==7427 || num==4025)
    {
      my.string("TRIPLE-3-IN",3,'c');;
      my.string("..NOR GATE..",4,'c');
    }
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    if(num==7411 || num==5411 || num==7410 || num==7412 || num==7427)
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 6:
          case 7:
          case 8:
          case 9:
          case 11:
          case 12:
            pinMode(p[i],OUTPUT);
            break;
          case 5:
          case 10:
          case 13:
            pinMode(p[i],INPUT);
            break;
        }
      }
    }
    else
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 12:
          case 13:
            pinMode(p[i],OUTPUT);
            break;
          case 10:
          case 9:
          case 11:
            pinMode(p[i],INPUT);
            break;
        }
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    while(x<=3)
    {
      is_ok=1;
      temp1=0;
      temp2=0;
      if(num==7411 || num==5411 || num==7410 || num==7412 || num==7427)
      {
        switch(x)
        {
          case 1:
            in1=0,in2=2,in3=3,out=5;
            break;
          case 2:
            in1=4,in2=6,in3=8,out=10;
            break;
          case 3:
            in1=7,in2=9,in3=11,out=13;
            break;
        }
      }
      else
      { 
        switch(x)
        {
          case 1:
            in1=0,in2=2,in3=13,out=11;
            break;
          case 2:
            in1=4,in2=6,in3=8,out=10;
            break;
          case 3:
            in1=3,in2=5,in3=7,out=9;
            break;
        }
      }
      my.erase();
      count=1;
      c=1;
      t=1;
      if(detl==1)
      {
        my.string("in  in  in out",0,'l');
        my.string("skp",5,'r');
        my.string("pse",5,'l');
        my.string("nxt",5,'c');
      }
      for(i=0;i<2;i++)
      {
        for(j=0;j<2;j++)
        {
          for(k=0;k<2;k++)
          {
            digitalWrite(p[in1],i);
            digitalWrite(p[in2],j);
            digitalWrite(p[in3],k);
              delay(1);
            state=digitalRead(p[out]);
              delay(1);
            if((detl==1)&&(temp2==0))
            {
              if(c==4 || c==7)
              {
                my.erase_row(1);
                my.erase_row(2);
                my.erase_row(3);
                count=1;
              }
              if(c==1)
                my.string("0   0   0",count,'l');
              else if(c==2)
                my.string("0   0   1",count,'l');
              else if(c==3)
                my.string("0   1   0",count,'l');
              else if(c==4)
                my.string("0   1   1",count,'l');
              else if(c==5)
                my.string("1   0   0",count,'l');
              else if(c==6)
                my.string("1   0   1",count,'l');
              else if(c==7)
                my.string("1   1   0",count,'l');
              else if(c==8)
                my.string("1   1   1",count,'l');
              my.dis_num(state,count,'r');
            }
            if(num==4073)
            {
              if(i==0 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==1 && state==0)
                is_ok=0;
            }
            if(num==4075)
            {
              if(i==0 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==0 && k==1 && state==0)
                is_ok=0;
              else if(i==0 && j==1 && k==0 && state==0)
                is_ok=0;
              else if(i==0 && j==1 && k==1 && state==0)
                is_ok=0;
              else if(i==1 && j==0 && k==0 && state==0)
                is_ok=0;
              else if(i==1 && j==0 && k==1 && state==0)
                is_ok=0;
              else if(i==1 && j==1 && k==0 && state==0)
                is_ok=0;
              else if(i==1 && j==1 && k==1 && state==0)
                is_ok=0;
            }
            if(num==7411 || num==5411)
            {
              if(i==0 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==1 && state==0)
                is_ok=0;
            }
            if(num==7410 || num==7412 || num==4023)
            {
              if(i==0 && j==0 && k==0 && state==0)
                is_ok=0;
              else if(i==0 && j==0 && k==1 && state==0)
                is_ok=0;
              else if(i==0 && j==1 && k==0 && state==0)
                is_ok=0;
              else if(i==0 && j==1 && k==1 && state==0)
                is_ok=0;
              else if(i==1 && j==0 && k==0 && state==0)
                is_ok=0;
              else if(i==1 && j==0 && k==1 && state==0)
                is_ok=0;
              else if(i==1 && j==1 && k==0 && state==0)
                is_ok=0;
              else if(i==1 && j==1 && k==1 && state!=0)
                is_ok=0;
            }
            if(num==7427 || num==4025)
            {
              if(i==0 && j==0 && k==0 && state==0)
                is_ok=0;
              else if(i==0 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==1 && state!=0)
                is_ok=0;
            }
            if((detl==1)&&(temp1==0)&&(temp2==0))
            {
              if((c%3)==0 || c==8)
              {
                while(temp1<350)
                {
                  delay(5);
                  key=inp.key_in();
                  if(key=='l')
                  {
                    my.erase_row(5);
                    my.string("cont",5,'c');
                    while(t)
                    {
                      delay(7);
                      key=inp.key_in();
                      if(key=='m')
                      {
                        t=0;
                        temp1=350;
                        my.erase_row(5);
                        my.string("skp",5,'r');
                        my.string("pse",5,'l');
                        my.string("nxt",5,'c');
                      }
                    }
                  }
                  else if(key=='m')
                  {
                    temp1=350;
                  }
                  else if(key=='r')
                  {
                    temp1=350;
                    temp2=350;
                  }
                  temp1++;
                }
                temp1=0;
                t=1;
              }
            }
            c++;
            count++;
          }
        }
      } 
      my.erase();
      delay(2);  
      my.string("GATE",0,'c');
      my.string("skp",5,'r');
      my.dis_num(x,2,'c');
      if(is_ok==1)
      {
        my.string("OK",4,'c');
        good_gate++;
      }
      else if(is_ok==0)
      {
        my.string("NOT OK",4,'c');
        bad_gate++;
      }
      temp1=0;
      while(temp1<300)
      {
        delay(8);
        key=inp.key_in();
        if(key=='r')
        {
          temp1=300;
        }
        temp1++;
      }
      x++;
    }
    result_gate(num);
} 
//###############################################################################
//I.C. 74_quad_2_input
int ic_74_quad_2(long int num)
{
    bhaskar:
    int i,j,detl=0,x=1;
    int count;
    int in1,in2,out,state,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    my.string("QUAD-2-INPUT",3,'c');
    if(num==7400 || num==7403 || num==74132)
      my.string("NAND GATE",4,'c');
    else if(num==7408 || num==5408 || num==7409 || num==5409)
      my.string("AND GATE",4,'c');
    else if(num==7432)
      my.string("OR GATE",4,'c');
    else if(num==7486)
      my.string("Ex-OR GATE",4,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          }
        }
      }
    }
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        case 9:
        case 11:
        case 12:
          pinMode(p[i],OUTPUT);
          break;
        case 4:
        case 10:
        case 7:
        case 13:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    
    x=1;
    while(x<=4)
    {
      is_ok=1;
      switch(x)
      {
        case 1:
          in1=0,in2=2,out=4;
          break;
        case 2:
          in1=6,in2=8,out=10;
          break;
        case 3:
          in1=9,in2=11,out=13;
          break;
        case 4:
          in1=3,in2=5,out=7;
          break;
      }
      my.erase();
      count=1;
      for(i=0;i<2;i++)
      {
        for(j=0;j<2;j++)
        {
          digitalWrite(p[in1],i);
          digitalWrite(p[in2],j);
          delay(2);
          state=digitalRead(p[out]);
          delay(2);
          if(detl==1)
          {
            my.string("IN",0,'l');
            my.string("IN",0,'c');
            my.string("OUT",0,'r');
            my.string("skp",5,'r');
            my.string("pse",5,'l');
            my.dis_num(i,count,'l');
            my.dis_num(j,count,'c');
            my.dis_num(state,count,'r');
          }
          if(num==7400 || num==7403 || num==74132)
          {
            if(i==0 && j==0 && state==0)
              is_ok=0;
            else if(i==0 && j==1 && state==0)
              is_ok=0;
            else if(i==1 && j==0 && state==0)
              is_ok=0;
            else if(i==1 && j==1 && state!=0)
              is_ok=0; 
          }
          else if(num==7408 || num==5408 || num==7409 || num==5409)
          {
            if(i==0 && j==0 && state!=0)
              is_ok=0;
            else if(i==0 && j==1 && state!=0)
              is_ok=0;
            else if(i==1 && j==0 && state!=0)
              is_ok=0;
            else if(i==1 && j==1 && state==0)
              is_ok=0; 
          }
          else if(num==7432)
          {
            if(i==0 && j==0 && state!=0)
              is_ok=0;
            else if(i==0 && j==1 && state==0)
              is_ok=0;
            else if(i==1 && j==0 && state==0)
              is_ok=0;
            else if(i==1 && j==1 && state==0)
              is_ok=0; 
          }
          else if(num==7486)
          {
            if(i==0 && j==0 && state!=0)
              is_ok=0;
            else if(i==0 && j==1 && state==0)
              is_ok=0;
            else if(i==1 && j==0 && state==0)
              is_ok=0;
            else if(i==1 && j==1 && state!=0)
              is_ok=0; 
          }
          count++;;
        }
      }
      int temp1=0,temp2=0;
      if(detl==1)
      { 
        while(temp1<350)
        {
          delay(9);
          key=inp.key_in();
          if(key=='r')
            temp1=350;
          else if(key=='l')
          {
            my.erase_row(5);
            my.string("cont",5,'c');
            while(temp2<350)
            {
              delay(7);
              key=inp.key_in();
              if(key=='m')
              {
                temp2=350;
                temp1=350;
              }
            }
          }
        temp1++;
        }
      }
      my.erase();
      delay(2);  
      my.string("GATE",0,'c');
      my.string("skp",5,'r');
      my.dis_num(x,2,'c');
      if(is_ok==1)
      {
        my.string("OK",4,'c');
        good_gate++;
      }
      else if(is_ok==0)
      {
        my.string("NOT OK",4,'c');
        bad_gate++;
      }
      temp1=0;
      while(temp1<300)
      {
        delay(8);
        key=inp.key_in();
        if(key=='r')
        {
          temp1=300;
        }
        temp1++;
      }
      x++;
    }
    result_gate(num);
    
}      
//###############################################################################
//I.C. hex_inverter
int ic_hex_inv(int num)
{
    bhaskar:
    int j,detl=0,x=1;
    int count;
    int in,out,state,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    my.string("HEX-INVERTER",3,'c');
    my.string("..NOT GATE..",4,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 3:
        case 4:
        case 7:
        case 8:
        case 11:
        case 12:
          pinMode(p[i],OUTPUT);
          break;
        case 2:
        case 5:
        case 6:
        case 9:
        case 10:
        case 13:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    
    x=1;
    while(x<=6)
    {
      is_ok=1;
      switch(x)
      {
        case 1:
          in=0,out=2;
          break;
        case 2:
          in=4,out=6;
          break;
        case 3:
          in=8,out=10;
          break;
        case 4:
          in=3,out=5;
          break;
        case 5:
          in=7,out=9;
          break;
        case 6:
          in=11,out=13;
          break;
      }
      my.erase();
      count=1;
      for(j=0;j<2;j++)
      {
        digitalWrite(p[in],j);
        delay(2);
        state=digitalRead(p[out]);
        delay(2);
        if(detl==1)
        {
          my.string("IN",0,'l');
          my.string("OUT",0,'r');
          my.string("skp",5,'r');
          my.string("pse",5,'l');
          my.dis_num(j,count,'l');
          my.dis_num(state,count,'r');
        }
        if(j==0 && state==0)
          is_ok=0;
        else if(j==1 && state!=0)
          is_ok=0;
        count++; 
      }
      int temp1=0,temp2=0;
      if(detl==1)
      { 
        while(temp1<350)
        {
          delay(9);
          key=inp.key_in();
          if(key=='r')
            temp1=350;
          else if(key=='l')
          {
            my.erase_row(5);
            my.string("cont",5,'c');
            while(temp2<350)
            {
              delay(7);
              key=inp.key_in();
              if(key=='m')
              {
                temp2=350;
                temp1=350;
              }
            }
          }
        temp1++;
        }
      }
      my.erase();
      delay(2);
      my.string("GATE",0,'c');
      my.string("skp",5,'r');
      my.dis_num(x,2,'c');
      if(is_ok==1)
      {
        my.string("OK",4,'c');
        good_gate++;
      }
      else if(is_ok==0)
      {
        my.string("NOT OK",4,'c');
        bad_gate++;
      }
      temp1=0;
      while(temp1<300)
      {
        delay(8);
        key=inp.key_in();
        if(key=='r')
        {
          temp1=300;
        }
        temp1++;
      }
      x++;
    }  
    result_gate(num);
} 
//###############################################################################
//IC_4029
int ic_4029()
{
    bhaskar:
    int i,j,y,x=1,is_ok=0,first_count=0,prev=0,ind=0,tem=0,arr[16];
    char mode,input;
    int count,temp,temp1=1,temp2=0;
    int bin=1,dec=1,in;
    int clock=1,bin_dec,up_down,out,dec_out;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(4029,1,'c');
    my.string("UP DWN counter",2,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        pra:
        x=0;
        y=1;
        count=1;
        my.erase();
        my.string("* Automatic",1,'l');
        my.string("* Manual",2,'l');
        my.string("* back",3,'l');
        my.string("(use up/down)",4,'c');
        my.string("ok",5,'c');
        my.string("<<",count,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='u')
          {
            if(count==2 || count==3)
            {
              my.erase_row(count);
              if(count==2)
                my.string("* Manual",2,'l');
              else if(count==3)
                my.string("* back",3,'l');  
              count--;
              my.string("<<",count,'r');
            }
          }
          else if(key=='d')
          {
            if(count==1 || count==2)
            {
              my.erase_row(count);
              if(count==1)
                my.string("* Automatic",1,'l');
              else if(count==2)
                my.string("* Manual",2,'l');  
              count++;
              my.string("<<",count,'r');
            }
          }
          else if(key=='m')
          {
            y=0;
            if(count==1)
              mode='a';
            else if(count==2)
              mode='m';
            else if(count==3)
              goto bhaskar;
          }
        }
      }
    } 
    x=1;
    int p[16]={34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
    for(i=0;i<16;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 13:
        case 14:
        case 15:
          pinMode(p[i],OUTPUT);
          break;
        case 2:
        case 5:
        case 10:
        case 11:
        case 12:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[14],0);
    digitalWrite(p[1],1); 
    
    if(mode=='m')
    { 
      go3:
      bin=1;dec=1;clock=1;
      y=1;
      count=2;
      my.erase();
      my.string("JAM INPUT",0,'c');
      my.string("* IN BINARY",2,'l');
      my.string("* IN DECIMAL",3,'l');
      my.string("(use up/down)",4,'c');
      my.string("ok",5,'c');
      my.string("bck",5,'r');
      my.string("<<",count,'r');
      while(y)
      {  
        key=inp.key_in();
        if(key=='u')
        {
          if(count==3)
          {
            my.erase_row(3);
            my.string("* IN DECIMAL",3,'l');
            count--;
            my.string("<<",count,'r');
          }
        }
        else if(key=='d')
        {
          if(count==2)
          {
            my.erase_row(2);
            my.string("* IN BINARY",2,'l'); 
            count++;
            my.string("<<",count,'r');
          }
        }
        else if(key=='m')
        {
            y=0;
            if(count==2)
              input='b';
            else if(count==3)
              input='d';
        }
        else if(key=='r')
          goto pra;
          
      }
    
      if(input=='b')
      {
        bin=accpt(input);
        in=bin;
      }
      else if(input=='d')
      {
        dec=accpt(input);
        in=dec2bin(dec);
      }
      if(bin==0 || dec==0)
      {
        if(ret==0)
          goto go3;
      }
      go4:
      clock=accpt('c');
      if(clock==0)
        goto go3;
      
      go5:
    
      y=1;
      count=2;
      my.erase();
      my.string("SELECT MODE",0,'c');
      my.string("* BINARY",2,'l');
      my.string("* DECADE",3,'l');
      my.string("(use up/down)",4,'c');
      my.string("ok",5,'c');
      my.string("bck",5,'r');
      my.string("<<",count,'r');
      while(y)
      {  
        key=inp.key_in();
        if(key=='u')
          {
            if(count==3)
            {
              my.erase_row(3);
              my.string("* DECADE",3,'l');
              count--;
              my.string("<<",count,'r');
            }
          }
          else if(key=='d')
          {
            if(count==2)
            {
              my.erase_row(2);
              my.string("* BINARY",2,'l');
              count++;
              my.string("<<",count,'r');
            }
          }
          else if(key=='m')
          {
              y=0;
              if(count==2)
                bin_dec=1;
              else if(count==3)
                bin_dec=0;
          }
          else if(key=='r')
            goto go4;
      }
    
      y=1;
      count=2;
      my.erase();
      my.string("SET UP/DOWN",0,'c');
      my.string("* UP",2,'l');
      my.string("* DOWN",3,'l');
      my.string("(use up/down)",4,'c');
      my.string("ok",5,'c');
      my.string("bck",5,'r');
      my.string("<<",count,'r');
      while(y)
      {  
        key=inp.key_in();
        if(key=='u')
        {
          if(count==3)
          {
            my.erase_row(3);
            my.string("* DOWN",3,'l');
            count--;
            my.string("<<",count,'r');
          }
        }
        else if(key=='d')
        {
          if(count==2)
          {
            my.erase_row(2);
            my.string("* UP",2,'l');
            count++;
            my.string("<<",count,'r');
          }
        }
        else if(key=='m')
        {
            y=0;
            if(count==2)
              up_down=1;
            else if(count==3)
              up_down=0;
        }
        else if(key=='r')
          goto go5;
      }
      
      int no=1;
      while(no<=4)
      {
        temp=in-(in/10)*10;
        if(no==1)
          digitalWrite(p[6],temp);
        else if(no==2)
          digitalWrite(p[9],temp);
        else if(no==3)
          digitalWrite(p[7],temp);
        else if(no==4)
          digitalWrite(p[4],temp);
        in=in/10;
        no++; 
      }
      digitalWrite(p[13],up_down);
      digitalWrite(p[15],bin_dec);
    
      digitalWrite(p[8],0);
      
      my.erase();
      my.string("..counting..",0,'c');
      my.string("dec",1,'l');
      my.string("bin",1,'r');
      my.string("---",2,'r');
      my.string("---",2,'l');
      my.string("up",1,'c');
      my.string("dwn",3,'c');
      my.string("stp",5,'r');
      my.string("rst",5,'l');
      my.string("pse",5,'c');
      go6:
      digitalWrite(p[0],HIGH);
      delay(1);
      digitalWrite(p[0],LOW);
      temp=1;
      first_count=0;
      while(temp)
      {
        my.erase_row(3);
        temp=digitalRead(p[2]);
          out=1000*temp;
          dec_out=temp*8;
        temp=digitalRead(p[5]);
          out=out+(100*temp);
          dec_out=dec_out+temp*4;
        temp=digitalRead(p[11]);
          out=out+(10*temp);
          dec_out=dec_out+temp*2;
        temp=digitalRead(p[10]);
          out=out+temp;
          dec_out=dec_out+temp;









        if(first_count==0)
        {
          prev=dec_out;
          first_count=1;
        }
        else
        {
          if((dec_out-prev)==1 || dec_out-prev==-1)
          {
            is_ok=1;
            prev=dec_out;
          }
          else
          {
            is_ok=0;
            prev=dec_out;
          }
        }














        
        itoa(out,str,10);  
        my.string(str,3,'r');
        
        itoa(dec_out,str,10);
        my.string(str,3,'l');
        
        my.string("dwn",3,'c');
        
        digitalWrite(p[3],HIGH);
        
        for(int i=1;i<=(clock/2);i++)
        {
          delay(1);
          key=inp.key_in();
          if(key=='l')
          {
            goto go6;
          }
          else if(key=='r')
          {
            goto go7;
          }
          else if(key=='m')
          {
            my.erase_row(5);
            my.string("cont",5,'c');
            temp1=1;
            while(temp1)
            {
              key=inp.key_in();
              if(key=='m')
              {
                temp1=0;
                my.erase_row(5);
                my.string("stp",5,'r');
                my.string("rst",5,'l');
                my.string("pse",5,'c');
              }
            }
          }
          else if(key=='u')
            digitalWrite(p[13],1);
          else if(key=='d')
            digitalWrite(p[13],0);
          else
            temp=digitalRead(p[12]);
        }
        
        
        digitalWrite(p[3],LOW);
        
        
        for(i=1;i<=(clock/2);i++)
        {
          delay(1);
          key=inp.key_in();
          if(key=='l')
          {
            goto go6;
          }
          else if(key=='r')
          {
            goto go7;
          }
          else if(key=='m')
          {
            my.erase_row(5);
            my.string("cont",5,'c');
            temp1=1;
            while(temp1)
            {
              delay(1);
              key=inp.key_in();
              if(key=='m')
              {
                temp1=0;
                my.erase_row(5);
                my.string("stp",5,'r');
                my.string("rst",5,'l');
                my.string("pse",5,'c');
              }
            }
          }
          else if(key=='u')
            digitalWrite(p[13],1);
          else if(key=='d')
            digitalWrite(p[13],0);
          else
            temp=digitalRead(p[12]);
        }
        
      }  
    }




    if(mode=='a')
    {
      my.erase();
      my.string("..Counting..",1,'c');
      my.string("Please wait",3,'c');
      digitalWrite(p[6],0);
      digitalWrite(p[9],0);
      digitalWrite(p[7],0);
      digitalWrite(p[4],0);
      digitalWrite(p[13],1);
      digitalWrite(p[0],1);
      delay(5);
      digitalWrite(p[0],0);

      digitalWrite(p[15],1);
      temp=1;
      ind=0;
      while(temp)
      {
        digitalWrite(p[3],0);
        tem=digitalRead(p[2]);
          dec_out=tem*8;
        tem=digitalRead(p[5]);
          dec_out=dec_out+tem*4;
        tem=digitalRead(p[11]);
          dec_out=dec_out+tem*2;
        tem=digitalRead(p[10]);
          dec_out=dec_out+tem;

        arr[ind]=dec_out;
        ind++;
        if(digitalRead(p[12])==0)
        {
          temp=0;
        } 
        delay(5);
        digitalWrite(p[3],1);
      }
      ind=0;
      //temp=1;
      while(ind<=15)
      {
        if(arr[ind]==ind)
        {
          is_ok=1; 
        }
        else
        {
          is_ok=0;
        }
        ind++;
      }
    }
    go7:
    delay(200);
    my.erase();
    my.string("Counting Ended",0,'c');
    my.string("--------------",1,'c');
    my.string("exit",5,'r');
    if(is_ok==1)
    {
      my.string("GOOD IC",3,'c');
    }
    else
    {
      my.string("BAD IC",3,'c');
    }
    temp=1;
    while(temp)
    {
      delay(2);
      key=inp.key_in();
      if(key=='r')
      {
        temp=0;
      }
    }
    
}
//IC_dual_4
//###############################################################################
int ic_dual_4(int num)
{
    bhaskar:
    int i,j,k,l,detl=0,x=1;
    int temp1=0,temp2=0;
    int count,c,t;
    int in1,in2,in3,in4,out,state,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    my.string("DUAL-4-INPUT",3,'c');
    if(num==4082 || num==7421 || num==5421)
      my.string("..AND GATE..",4,'c');
    else if(num==4012 || num==7420)
      my.string("..NAND GATE..",4,'c');
    else if(num==4002)
      my.string("..NOR GATE..",4,'c');
    else if(num==4072)
      my.string("..OR GATE..",4,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    if(num==7421 || num==5421 || num==7420)
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 0:
          case 1:
          case 2:
          case 3:
          case 5:
          case 6:
          case 8:
          case 9:
          case 11:
          case 12:
            pinMode(p[i],OUTPUT);
            break;
          case 10:
          case 13:
            pinMode(p[i],INPUT);
            break;
        }
      }
    }
    else if(num==4082 || num==4012 || num==4002 || num==4072)
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 1:
          case 2:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
          case 11:
          case 12:
            pinMode(p[i],OUTPUT);
            break;
          case 0:
          case 3:
            pinMode(p[i],INPUT);
            break;
        }
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    while(x<=2)
    {
      is_ok=1;
      temp1=0;
      temp2=0;
      if(num==7421 || num==5421 || num==7420)
      {
        switch(x)
        {
          case 1:
            in1=0,in2=2,in3=6,in4=8,out=10;
            break;
          case 2:
            in1=3,in2=5,in3=9,in4=11,out=13;
            break;
        }
      }
      else if(num==4082 || num==4012 || num==4002 || num==4072)
      {
        switch(x)
        {
          case 1:
            in1=2,in2=4,in3=6,in4=8,out=0;
            break;
          case 2:
            in1=11,in2=9,in3=7,in4=5,out=3;
            break;
        }
      }
      my.erase();
      count=1;
      c=1;
      t=1;
      if(detl==1)
      {
        my.string("A  B  C  D",0,'l');
        my.string("Y",0,'r');
        my.string("skp",5,'r');
        my.string("pse",5,'l');
        my.string("nxt",5,'c');
      }
      for(i=0;i<2;i++)
      {
        for(j=0;j<2;j++)
        {
          for(k=0;k<2;k++)
          {
            for(l=0;l<2;l++)
            {
              digitalWrite(p[in1],i);
              digitalWrite(p[in2],j);
              digitalWrite(p[in3],k);
              digitalWrite(p[in4],l);
                delay(1);
              state=digitalRead(p[out]);
                delay(1);
              if((detl==1)&&(temp2==0))
              {
                if(c==4 || c==7 || c==10 || c==13 || c==16)
                {
                  my.erase_row(1);
                  my.erase_row(2);
                  my.erase_row(3);
                  count=1;
                }
                if(c==1)
                  my.string("0  0  0  0",count,'l');
                else if(c==2)
                  my.string("0  0  0  1",count,'l');
                else if(c==3)
                  my.string("0  0  1  0",count,'l');
                else if(c==4)
                  my.string("0  0  1  1",count,'l');
                else if(c==5)
                  my.string("0  1  0  0",count,'l');
                else if(c==6)
                  my.string("0  1  0  1",count,'l');
                else if(c==7)
                  my.string("0  1  1  0",count,'l');
                else if(c==8)
                  my.string("0  1  1  1",count,'l');
                else if(c==9)
                  my.string("1  0  0  0",count,'l');
                else if(c==10)
                  my.string("1  0  0  1",count,'l');
                else if(c==11)
                  my.string("1  0  1  0",count,'l');
                else if(c==12)
                  my.string("1  0  1  1",count,'l');
                else if(c==13)
                  my.string("1  1  0  0",count,'l');
                else if(c==14)
                  my.string("1  1  0  1",count,'l');
                else if(c==15)
                  my.string("1  1  1  0",count,'l');
                else if(c==16)
                  my.string("1  1  1  1",count,'l');
                my.dis_num(state,count,'r');
              }
              if(num==7421 || num==5421 || num==4082 )
              {
                if(i==0 && j==0 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==0 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==1 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==1 && state==0)
                  is_ok=0;
              }
              if(num==4012 || num==7420)
              {
                if(i==0 && j==0 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==0 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==1 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==1 && state!=0)
                  is_ok=0;
              }
              if(num==4002)
              {
                if(i==0 && j==0 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==0 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==1 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==1 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==0 && state!=0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==1 && state!=0)
                  is_ok=0;
              }
              if(num==4072)
              {
                if(i==0 && j==0 && k==0 && l==0 && state!=0)
                  is_ok=0;
                else if(i==0 && j==0 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==0 && k==1 && l==1 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==0 && j==1 && k==1 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==0 && k==1 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==0 && l==1 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==0 && state==0)
                  is_ok=0;
                else if(i==1 && j==1 && k==1 && l==1 && state==0)
                  is_ok=0;
              }
              if((detl==1)&&(temp1==0)&&(temp2==0))
              {
                if((c%3)==0 || c==16)
                {
                  while(temp1<350)
                  {
                    delay(5);
                    key=inp.key_in();
                    if(key=='l')
                    {
                      my.erase_row(5);
                      my.string("cont",5,'c');
                      while(t)
                      {
                        delay(7);
                        key=inp.key_in();
                        if(key=='m')
                        {
                          t=0;
                          temp1=350;
                          my.erase_row(5);
                          my.string("skp",5,'r');
                          my.string("pse",5,'l');
                          my.string("nxt",5,'c');
                        }
                      }
                    }
                    else if(key=='m')
                    {
                      temp1=350;
                    }
                    else if(key=='r')
                    {
                      temp1=350;
                      temp2=350;
                    }
                    temp1++;
                  }
                  temp1=0;
                  t=1;
                }
              }
              c++;
              count++;
            }
          }
        }
      } 
      my.erase();
      delay(2);  
      my.string("GATE",0,'c');
      my.string("skp",5,'r');
      my.dis_num(x,2,'c');
      if(is_ok==1)
      {
        my.string("OK",4,'c');
        good_gate++;
      }
      else if(is_ok==0)
      {
        my.string("NOT OK",4,'c');
        bad_gate++;
      }
      temp1=0;
      while(temp1<300)
      {
        delay(8);
        key=inp.key_in();
        if(key=='r')
        {
          temp1=300;
        }
        temp1++;
      }
      x++;
    }
    result_gate(7421);
    
} 
//###############################################################################
//IC_oct_1
int ic_oct_1(int num)
{
    bhaskar:
    int a,b,c,d,e,f,g,h,x=1,temp1;
    int in1,in2,in3,in4,in5,in6,in7,in8,out1,out2,state,is_ok=1,is_ok1=1,is_ok2=1;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    my.string("8-INPUT",3,'c');
    if(num==7430)
      my.string("..NAND GATE..",4,'c');
    else if(num==4068)
      my.string("NAND + AND",4,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        my.erase();
        my.string("Checking...",2,'c');
      }
    }
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    if(num==7430)
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 0:
          case 1:
          case 2:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 10:
          case 12:
            pinMode(p[i],OUTPUT);
            break;
          case 13:
            pinMode(p[i],INPUT);
            break;
        }
      }
    }
    else if(num==4068)
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 1:
          case 2:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
          case 11:
          case 12:
            pinMode(p[i],OUTPUT);
            break;
          case 0:
          case 3:
            pinMode(p[i],INPUT);
            break;
        }
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    if(num==7430)
    {
      in1=0,in2=2,in3=4,in4=5,in5=6,in6=7,in7=8,in8=10;
      out1=13;
    }
    else if(num==4068)
    {
      in1=2,in2=4,in3=5,in4=6,in5=7,in6=8,in7=9,in8=11;
      out1=0;
      out2=3;
    }
    for(a=0;a<2;a++)
    {
      for(b=0;b<2;b++)
      {
        for(c=0;c<2;c++)
        {
          for(d=0;d<2;d++)
          {
            for(e=0;e<2;e++)
            {
              for(f=0;f<2;f++)
              {
                for(g=0;g<2;g++)
                {
                  for(h=0;h<2;h++)
                  {
                    digitalWrite(p[in1],a);
                    digitalWrite(p[in2],b);
                    digitalWrite(p[in3],c);
                    digitalWrite(p[in4],d);
                    digitalWrite(p[in5],e);
                    digitalWrite(p[in6],f);
                    digitalWrite(p[in7],g);
                    digitalWrite(p[in8],h);
                    delay(1);
                    if(num==7430)
                    {
                      state=digitalRead(p[out1]);
                      if(a==1 && b==1 && c==1 && d==1 && e==1 && f==1 && g==1 && h==1)
                      {
                        if(state!=0)
                          is_ok=0;
                      }
                      else 
                      {
                        if(state==0)
                          is_ok=0;
                      }  
                    }
                    else if(num==4068)
                    {
                      state=digitalRead(p[out1]);
                      if(a==1 && b==1 && c==1 && d==1 && e==1 && f==1 && g==1 && h==1)
                      {
                        if(state==0)
                          is_ok1=0;
                      }
                      else 
                      {
                        if(state!=0)
                          is_ok1=0;
                      }  
                      state=digitalRead(p[out2]);
                      if(a==1 && b==1 && c==1 && d==1 && e==1 && f==1 && g==1 && h==1)
                      {
                        if(state!=0)
                          is_ok2=0;
                      }
                      else 
                      {
                        if(state==0)
                          is_ok2=0;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    if(num==4068)
    {
      my.erase();
      my.string("RESULT",0,'c');
      my.string("------",1,'c');
      my.string("exit",5,'r');
      if(is_ok1==1)
        my.string("AND gate OK",2,'c');
      else if(is_ok1==0)
        my.string("AND gate BAD",2,'c');
      if(is_ok2==1)
        my.string("NAND gate OK",3,'c');
      else if(is_ok2==0)
        my.string("NAND gate BAD",3,'c');
      my.string("I.C.",4,'l');
      my.dis_num(4068,4,'c');
      if(is_ok1==1 && is_ok2==1)
        my.string("GOOD",4,'r');
      else
        my.string("BAD",4,'r');
      while(1)
      {
        delay(7);
        key=inp.key_in();
        if(key=='r')
        {
            return 0;
        }
      } 
    }
    else if(num==7430)
    {
      my.erase();
      my.string("skp",5,'r');
      my.string("GATE",0,'c');
      my.string("is",2,'c');
      if(is_ok==1)
        my.string("OK",4,'c');
      else if(is_ok==0)
        my.string("NOT OK",4,'c');
      temp1=0;
      while(temp1<300)
      {
        delay(8);
        key=inp.key_in();
        if(key=='r')
        {
          temp1=300;
        }
        temp1++;
      }  
      my.erase();
      my.string("RESULT",0,'c');
      my.string("------",1,'c');
      my.string("exit",5,'r');
      if(is_ok==1)
        my.string("Gate is GOOD",2,'c');
      else if(is_ok==0)
        my.string("Gate is BAD",2,'c');
      my.string("I.C.",4,'l');
      my.dis_num(7430,4,'c');
      if(is_ok==1)
        my.string("GOOD",4,'r');
      else
        my.string("BAD",4,'r');
      while(1)
      {
        delay(7);
        key=inp.key_in();
        if(key=='r')
        {
            return 0;
        }
      } 
    }
} 
//###############################################################################
//IC_74393
int ic_74393()
{
    bhaskar:
    int i,j,x=1,kk;
    int temp,temp1=1,state;
    int clock1=1,clock2=1,out1=0,dec_out1=0,out2=0,dec_out2=0,ind1=0,ind2=0;

    int is_ok_c1=0, is_ok_c2=0;
    int arr1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int arr2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    char str1[5]="---";
    char str2[5]="---";
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(74393,1,'c');
    my.string("Counter",3,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
      }
    } 
    x=1;
    go2:
    my.erase();
    my.string("For COUNTER 1:",1,'c');
    my.string("--------------",2,'c');
    delay(1000);
    clock1=accpt('c');
    if(clock1==0)
      goto bhaskar;
    my.erase();
    my.string("For COUNTER 2:",1,'c');
    my.string("--------------",2,'c');
    delay(1000);
    clock2=accpt('c');
    if(clock2==0)
      goto go2;
    int p[16]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 12:
          pinMode(p[i],OUTPUT);
          break;
        case 4:
        case 6:
        case 8:
        case 10:
        case 7:
        case 9:
        case 11:
        case 13:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    
    digitalWrite(p[3],0);
    digitalWrite(p[0],0);
    
    digitalWrite(p[2],1);
    digitalWrite(p[5],1);
    my.erase();
    my.string("..counting..",0,'c');
    my.string("C1",1,'l');
    my.string("C2",1,'r');
    my.string("--",2,'r');
    my.string("--",2,'l');
    my.string("rst1",2,'c');
    my.string("rst2",4,'c');
    my.string("stp",5,'r');
    my.string("pse",5,'l');
    delay(5);
    digitalWrite(p[2],0);
    digitalWrite(p[5],0);
    temp=1;
    ind1=0;
    ind2=0;
    while(temp)
    {  
      for(i=1;(i<=clock1 || i<=clock2);i++)
      {
        delay(1);
        key=inp.key_in();
        if(key=='r')
        {
          goto go7;
        }
        else if(key=='l')
        {
          my.erase_row(5);
          my.string("cont",5,'c');
          temp1=1;
          while(temp1)
          {
            key=inp.key_in();
            if(key=='m')
            {
              temp1=0;
              my.erase_row(5);
              my.string("stp",5,'r');
              my.string("pse",5,'l');
            }
          }
        }
        else if(key=='u')
        {
          digitalWrite(p[2],1);
          delay(1);
          digitalWrite(p[2],0);
          my.erase_row(3);
          my.string(str2,3,'r');
          for(kk=0;kk<=15;kk++)
          {
            arr1[kk]=0;
          }
          ind1=1;
        }
        else if(key=='d')
        {
          digitalWrite(p[5],1);
          delay(1);
          digitalWrite(p[5],0);
          my.erase_row(3);
          my.string(str1,3,'l');
          for(kk=0;kk<=15;kk++)
          {
            arr2[kk]=0;
          }
          ind2=1;
        }
        if((i%clock1)==0 && out1<1111)
        {
          digitalWrite(p[0],0);
          state=digitalRead(p[10]);
            out1=state*1000;
            dec_out1=state*8;
          state=digitalRead(p[8]);
            out1=out1+(state*100);
            dec_out1=dec_out1+(state*4);
          state=digitalRead(p[6]);
            out1=out1+(state*10);
            dec_out1=dec_out1+(state*2);
          state=digitalRead(p[4]);
            out1=out1+state;
            dec_out1=dec_out1+state;
            
          arr1[ind1]=dec_out1;
          ind1++;
          
          itoa(out1,str1,10);
          my.erase_row(3);
          my.string(str1,3,'l');
          my.string(str2,3,'r');
          digitalWrite(p[0],1);
        }
        if((i%clock2)==0 && out2<1111)
        {
          digitalWrite(p[3],0);
          state=digitalRead(p[13]);
            out2=state*1000;
            dec_out2=state*8;
          state=digitalRead(p[11]);
            out2=out2+(state*100);
            dec_out2=dec_out2+(state*4);
          state=digitalRead(p[9]);
            out2=out2+(state*10);
            dec_out2=dec_out2+(state*2);
          state=digitalRead(p[7]);
            out2=out2+state;
            dec_out2=dec_out2+state;

          arr2[ind2]=dec_out2;
          ind2++;
          
          itoa(out2,str2,10);
          my.erase_row(3);
          my.string(str1,3,'l');
          my.string(str2,3,'r');
          digitalWrite(p[3],1);
        }
      } 
      if(ind1>15 && ind2>15)
        temp=0; 
    }
    
    delay(300);
    my.erase();
    my.string("Counting Ended",1,'c');
    my.string("--------------",2,'c');
    delay(500);
    my.erase();
    if(ind1>=5)
    {
      for(i=1;i<(ind1-1);i++)
      {
        if((arr1[i+1]-arr1[i])==1)
        {
          is_ok_c1=1;
        }
        else
        {
          is_ok_c1=0;
          break;
        }
      }
    }
    if(ind2>=5)
    {  
      for(i=1;i<(ind2-1);i++)
      {
        if((arr2[i+1]-arr2[i])==1)
        {
          is_ok_c2=1;
        }
        else
        {
          is_ok_c2=0;
          break;
        }
      }
    }

    go7:
    my.erase();
    if(ind1<5 || ind2<5)
    {
      my.string("Try again...",3,'c');
      delay(1000);
      return 0;
    }

    
    if(is_ok_c1==1)
    {
      my.string("Count1 OK",2,'c');
    }
    else
    {
      my.string("Count1 BAD",2,'c');
    }

    if(is_ok_c2==1)
    {
      my.string("Count2 OK",3,'c');
    }
    else
    {
      my.string("Count2 BAD",3,'c');
    }
    if(is_ok_c2==1 && is_ok_c1==1)
    {
      my.string("74393 OK",4,'c');
    }
    else
    {
      my.string("74393 BAD",4,'c');
    }
    
    my.string("RESULT",0,'c');
    my.string("------",1,'c');
    my.string("back",5,'r');
    temp=1;
    while(temp)
    {
      delay(5);
      key=inp.key_in();
      if(key=='r')
      {
        temp=0;
        break;
      }
    }
}
//###############################################################################
//ic_hex_16pin
int ic_hex_16pin(int num)
{
    bhaskar:
    int j,detl=0,x=1;
    int count;
    int in,out,state,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    if(num==4050)
    {
      my.string("HEX-BUFFER",3,'c');
      my.string("non-inverting",4,'c');
    }
    else if(num==4049)
    {
      my.string("HEX-INVERTER",3,'c');
      my.string("..NOT GATE..",4,'c');
    }
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(5);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }
    x=1;
    int p[16]={34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
    for(i=0;i<16;i++)
    {
      switch(i)
      {  
        case 0:
        case 4:
        case 8:
        case 12:
        case 15:
        case 11:
        case 5:
        case 14:
          pinMode(p[i],OUTPUT);
          break;
        case 2:
        case 6:
        case 10:
        case 13:
        case 9:
        case 3:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[14],0);
    digitalWrite(p[0],1); 
    
    x=1;
    while(x<=6)
    {
      is_ok=1;
      switch(x)
      {
        case 1:
          in=4,out=2;
          break;
        case 2:
          in=8,out=6;
          break;
        case 3:
          in=12,out=10;
          break;
        case 4:
          in=15,out=13;
          break;
        case 5:
          in=11,out=9;
          break;
        case 6:
          in=5,out=3;
          break;
      }
      my.erase();
      count=1;
      for(j=0;j<2;j++)
      {
        digitalWrite(p[in],j);
        delay(1);
        state=digitalRead(p[out]);
        delay(1);
        if(detl==1)
        {
          my.string("IN",0,'l');
          my.string("OUT",0,'r');
          my.string("skp",5,'r');
          my.string("pse",5,'l');
          my.dis_num(j,count,'l');
          my.dis_num(state,count,'r');
        }
        if(num==4049)
        {
          if(j==0 && state==0)
            is_ok=0;
          else if(j==1 && state!=0)
            is_ok=0;
        }
        else if(num==4050)
        {
          if(j==0 && state!=0)
            is_ok=0;
          else if(j==1 && state==0)
            is_ok=0;
        }
        count++;    
    }
    int temp1=0,temp2=0;
    if(detl==1)
    { 
      while(temp1<350)
      {
        delay(9);
        key=inp.key_in();
        if(key=='r')
          temp1=350;
        else if(key=='l')
        {
          my.erase_row(5);
          my.string("cont",5,'c');
          while(temp2<350)
          {
            delay(7);
            key=inp.key_in();
            if(key=='m')
            {
              temp2=350;
              temp1=350;
            }
          }
        }
      temp1++;
      }
    }
    my.erase();
    delay(1);
    my.string("GATE",0,'c');
    my.string("skp",5,'r');
    my.dis_num(x,2,'c');
    if(is_ok==1)
    {
      my.string("OK",4,'c');
      good_gate++;
    }
    else if(is_ok==0)
    {
      my.string("NOT OK",4,'c');
      bad_gate++;
    }
    temp1=0;
    while(temp1<300)
    {
      delay(8);
      key=inp.key_in();
      if(key=='r')
      {
        temp1=300;
      }
      temp1++;
    }
    x++;
  }  
  result_gate(num);
}
//###############################################################################
//ic_4000
int ic_4000()
{
  bhaskar:
  int i,j,k,detl=0,x=1,c,t;
  int count;
  int in,out,state,is_ok=1;
  int in1,in2,in3;
  int temp1=0,temp2=0;
  my.erase();
  my.string("PLACE THE IC",0,'c');
  my.dis_num(4000,1,'c');
  my.string("Dual-3-in NOR",3,'c');
  my.string("+ NOT",4,'c');
  my.string("ext",5,'l');
  my.string("chk",5,'c');
  my.string("bck",5,'r');
  while(x)
  {
    delay(1);
    key=inp.key_in();
    if(key=='l')
    {
      return 0;
    }
    else if(key=='r')
    {
      return 1;
    }
    else if(key=='m')
    {
      x=0;
      int y=1;
      my.erase();
      my.string("TRUTH TABLE",1,'c');
      my.string("-----------",2,'c');
      my.string("????",3,'c');
      my.string("yes",5,'c');
      my.string("no",5,'l');
      my.string("bck",5,'r');
      while(y)
      {
        key=inp.key_in();
        if(key=='m')
        {
          detl=1;
          y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }    
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 4:
        case 6:
        case 8:
        case 3:
        case 5:
        case 7:
        case 13:
          pinMode(p[i],OUTPUT);
          break;
        case 10:
        case 11:
        case 9:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    x=1;
    while(x<=3)
    {
      temp1=0,temp2=0;
      is_ok=1;
      switch(x)
      {
        case 1:
          in1=4,in2=6,in3=8,out=10;
          break;
        case 2:
          in1=13,out=11;
          break;
        case 3:
          in1=3,in2=5,in3=7,out=9;
          break;
      }
      my.erase();
      if(x==1 || x==3)
      {
        
        count=1;
        c=1;
        t=1;
        if(detl==1)
        {
          my.string("in  in  in out",0,'l');
          my.string("skp",5,'r');
          my.string("pse",5,'l');
          my.string("nxt",5,'c');
        }
        for(i=0;i<2;i++)
        {
          for(j=0;j<2;j++)
          {
            for(k=0;k<2;k++)
            {
              digitalWrite(p[in1],i);
              digitalWrite(p[in2],j);
              digitalWrite(p[in3],k);
                delay(1);
              state=digitalRead(p[out]);
                delay(1);
              if((detl==1)&&(temp2==0))
              {
                if(c==4 || c==7)
                {
                  my.erase_row(1);
                  my.erase_row(2);
                  my.erase_row(3);
                  count=1;
                }
                if(c==1)
                  my.string("0   0   0",count,'l');
                else if(c==2)
                  my.string("0   0   1",count,'l');
                else if(c==3)
                  my.string("0   1   0",count,'l');
                else if(c==4)
                  my.string("0   1   1",count,'l');
                else if(c==5)
                  my.string("1   0   0",count,'l');
                else if(c==6)
                  my.string("1   0   1",count,'l');
                else if(c==7)
                  my.string("1   1   0",count,'l');
                else if(c==8)
                  my.string("1   1   1",count,'l');
                my.dis_num(state,count,'r');
              }
              if(i==0 && j==0 && k==0 && state==0)
                is_ok=0;
              else if(i==0 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==0 && j==1 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==0 && k==1 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==0 && state!=0)
                is_ok=0;
              else if(i==1 && j==1 && k==1 && state!=0)
                is_ok=0;
              if((detl==1)&&(temp1==0)&&(temp2==0))
              {
                if((c%3)==0 || c==8)
                {
                  while(temp1<350)
                  {
                    delay(5);
                    key=inp.key_in();
                    if(key=='l')
                    {
                      my.erase_row(5);
                      my.string("cont",5,'c');
                      while(t)
                      {
                        delay(1);
                        key=inp.key_in();
                        if(key=='m')
                        {
                          t=0;
                          temp1=350;
                          my.erase_row(5);
                          my.string("skp",5,'r');
                          my.string("pse",5,'l');
                          my.string("nxt",5,'c');
                        }
                      }
                    }
                    else if(key=='m')
                    {
                      temp1=350;
                    }
                    else if(key=='r')
                    {
                      temp1=350;
                      temp2=350;
                    }
                    temp1++;
                  }
                  temp1=0;
                  t=1;
                }
              }
              c++;
              count++;
            }
          }
        } 
      }
      else if(x==2)
      {
        count=1;
        for(j=0;j<2;j++)
        {
          digitalWrite(p[in],j);
          delay(1);
          state=digitalRead(p[out]);
          delay(1);
          if(detl==1)
          {
            my.string("IN",0,'l');
            my.string("OUT",0,'r');
            my.string("skp",5,'r');
            my.string("pse",5,'l');
            my.dis_num(j,count,'l');
            my.dis_num(state,count,'r');
          }
          if(j==0 && state==0)
            is_ok=0;
          else if(j==1 && state!=0)
            is_ok=0;
          count++;   
        }
        temp1=0,temp2=0;
        if(detl==1)
        { 
          while(temp1<350)
          {
            delay(5);
            key=inp.key_in();
            if(key=='r')
              temp1=350;
            else if(key=='l')
            {
              my.erase_row(5);
              my.string("cont",5,'c');
              while(temp2<350)
              {
                delay(7);
                key=inp.key_in();
                if(key=='m')
                {
                  temp2=350;
                  temp1=350;
                }
              }
            }
          temp1++;
          }
        }
      }
      my.erase();
      delay(1);
      if(x==2)
        my.string("NOT ( 2nd )",0,'c');
      else if(x==1)
        my.string("NOR ( 1st )",0,'c');
      else if(x==3)
        my.string("NOR ( 3rd )",0,'c');
      my.string("GATE",2,'c');
      my.string("skp",5,'r');
      if(is_ok==1)
      {
        my.string("OK",4,'c');
        good_gate++;
      }
      else if(is_ok==0)
      {
        my.string("NOT OK",4,'c');
        bad_gate++;
      }  
     
      temp1=0;
      while(temp1<300)
      {
        delay(8);
        key=inp.key_in();
        if(key=='r')
        {
          temp1=300;
        }
        temp1++;
      }
      x++;
    }  
}
//###############################################################################
int ic_d_flip(int num)
{
    bhaskar:
    int j,detl=0,x=1;
    int state,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    my.string("Dual D",3,'c');
    my.string("..flip-flop..",4,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(1);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }
    x=1;
    int p[14]={34,35,36,37,38,39,40,41,42,43,44,45,46,47};
    for(i=0;i<14;i++)
    {
      switch(i)
      {  
        case 0:
        case 2:
        case 4:
        case 6:
        case 3:
        case 5:
        case 7:
        case 9:
          pinMode(p[i],OUTPUT);
          break;
        case 8:
        case 10:
        case 11:
        case 13:
          pinMode(p[i],INPUT);
          break;
      }
    }
    digitalWrite(p[12],0);
    digitalWrite(p[1],1); 
    
    x=1;
    int rst,d,clk,set,q,qbar;
    while(x<=2)
    {
      is_ok=1;
      switch(x)
      {
        case 1:
          rst=0,d=2,clk=4,set=6,q=8,qbar=10;
          break;
        case 2:
          rst=3,d=5,clk=7,set=9,q=11,qbar=13;
          break;
      }
      if(detl==1)
      {
        my.erase();
        my.string("D",0,'l');
        my.string("CLK",1,'l');
        my.string("Q",2,'l');
        my.string("Q_bar",3,'l');
        my.string("SET",4,'l');
        my.string("RESET",5,'l');
      }
      for(j=0;j<2;j++)
      {
        digitalWrite(p[d],j);
        digitalWrite(p[clk],1);
        digitalWrite(p[rst],1);
        digitalWrite(p[set],1);
        
        if(detl==1)
        {
          my.dis_num(j,0,'c');
          my.dis_num(1,1,'c');
          my.dis_num(1,4,'c');
          my.dis_num(1,5,'c');
          my.dis_num(digitalRead(p[q]),2,'c');
          my.dis_num(digitalRead(p[qbar]),3,'c');
          delay(2000);
        }
        digitalWrite(p[clk],0);
        delay(5);
        if(detl==1)
        {
          my.dis_num(0,1,'c');
          my.dis_num(digitalRead(p[q]),2,'c');
          my.dis_num(digitalRead(p[qbar]),3,'c');
          delay(2000);
        }
        digitalWrite(p[clk],1);
        if(j==0)
        {
          if((digitalRead(p[q])!=0) || (digitalRead(p[qbar])!=1))
            is_ok=0;
        }
        if(j==1)
        {
          if((digitalRead(p[q])!=1) || (digitalRead(p[qbar])!=0))
            is_ok=0;
        }
        if(detl==1)
        {
          my.dis_num(1,1,'c');
          my.dis_num(digitalRead(p[q]),2,'c');
          my.dis_num(digitalRead(p[qbar]),3,'c');
          delay(2000);
        }
      }
      for(j=0;j<2;j++)
      {
        digitalWrite(p[rst],0);
        digitalWrite(p[set],1);
        if((digitalRead(p[q])!=0) || (digitalRead(p[qbar])!=1))
          is_ok=0;
        if(detl==1)
        {
          my.dis_num(1,4,'c');
          my.dis_num(0,5,'c');
          my.dis_num(digitalRead(p[q]),2,'c');
          my.dis_num(digitalRead(p[qbar]),3,'c');
          delay(2000);
        }
        digitalWrite(p[rst],1);
        digitalWrite(p[set],0);
        if((digitalRead(p[q])!=1) || (digitalRead(p[qbar])!=0))
          is_ok=0;
        if(detl==1)
        {
          my.dis_num(0,4,'c');
          my.dis_num(1,5,'c');
          my.dis_num(digitalRead(p[q]),2,'c');
          my.dis_num(digitalRead(p[qbar]),3,'c');
          delay(2000);
        }
      }
      my.erase();
      my.string("FLIP FLOP",1,'c');
      my.dis_num(x,3,'c');
      if(is_ok==1)
        my.string("GOOD",5,'c');
      else
        my.string("BAD",5,'c');
      delay(2000);
      x++;
    }
}       

//###############################################################################
int ic_jk_flip(int num)
{
    bhaskar:
    int r,s,t,detl=0,x=1;
    int state_q,state_qbar,is_ok;
    my.erase();
    my.string("PLACE THE IC",0,'c');
    my.dis_num(num,1,'c');
    my.string("Dual JK",3,'c');
    my.string("..flip-flop..",4,'c');
    my.string("ext",5,'l');
    my.string("chk",5,'c');
    my.string("bck",5,'r');
    while(x)
    {
      delay(1);
      key=inp.key_in();
      if(key=='l')
      {
        return 0;
      }
      else if(key=='r')
      {
        return 1;
      }
      else if(key=='m')
      {
        x=0;
        int y=1;
        my.erase();
        my.string("TRUTH TABLE",1,'c');
        my.string("-----------",2,'c');
        my.string("????",3,'c');
        my.string("yes",5,'c');
        my.string("no",5,'l');
        my.string("bck",5,'r');
        while(y)
        {
          key=inp.key_in();
          if(key=='m')
          {
            detl=1;
            y=0;
          }
          else if(key=='l')
          {
            detl=0;
            y=0;
          }
          else if(key=='r')
          {
            goto bhaskar;
          } 
        }
      }
    }
    x=1;
    int p[16]={34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
    if(num==7473)
    {
      for(i=0;i<14;i++)
      {
        switch(i)
        {  
          case 0:
          case 1:
          case 2:
          case 4:
          case 8:
          case 6:
          case 7:
          case 9:
          case 10:
          case 12:
            pinMode(p[i],OUTPUT);
            break;
          case 3:
          case 5:
          case 11:
          case 13:
            pinMode(p[i],INPUT);
            break;
        }
      }
      digitalWrite(p[7],0);
      digitalWrite(p[6],1); 
    }
    if(num==7476)
    {
      for(i=0;i<16;i++)
      {
        switch(i)
        {  
          case 0:
          case 2:
          case 4:
          case 6:
          case 8:
          case 10:
          case 12:
          case 14:
          case 1:
          case 7:
          case 9:
          case 15:
            pinMode(p[i],OUTPUT);
            break;
          case 3:
          case 5:
          case 11:
          case 13:
            pinMode(p[i],INPUT);
            break;
        }
      }
      digitalWrite(p[7],0);
      digitalWrite(p[8],1); 
    }
    x=1;
    int rst,j,k,clk,set,q,qbar;
    while(x<=2)
    {
      is_ok=1;
      if(num==7476)
      {
        switch(x)
        {
          case 1:
            rst=4,j=6,k=1,clk=0,set=2,q=3,qbar=5;
            break;
          case 2:
            rst=14,j=15,k=9,clk=10,set=12,q=11,qbar=13;
            break;
        }
      }
      if(num==7473)
      {
        switch(x)
        {
          case 1:
            rst=2,j=1,k=4,clk=0,q=5,qbar=3;
            break;
          case 2:
            rst=10,j=12,k=9,clk=8,q=11,qbar=13;
            break;
        }
      }
      if(detl==1)
      {
        my.erase();
        my.string("J",0,'l');
        my.string("K",1,'l');
        my.string("CLK",2,'l');
        my.string("Q",3,'l');
        my.string("Q_bar",4,'l');
      }
      for(t=0;t<=4;t++)
      {
        me:
        switch(t)
        {
          case 1:
            r=1,s=0;
            break;
          case 2:
            r=0,s=0;
            break;
          case 3:
            r=0,s=1;
            break;
          case 4:
            r=1,s=1;
            break;
        }
        if(t==0)
        {
          digitalWrite(p[rst],0);
          if(num==7476)
            digitalWrite(p[set],1);
          if(detl==1)
          {  
            my.string("-Resetting-",5,'c');
            my.string("-",0,'c');
            my.string("-",1,'c');
            my.string("-",2,'c');
            my.dis_num(digitalRead(p[q]),3,'c');
            my.dis_num(digitalRead(p[qbar]),4,'c');
            delay(2000);
            my.erase();
            my.string("J",0,'l');
            my.string("K",1,'l');
            my.string("CLK",2,'l');
            my.string("Q",3,'l');
            my.string("Q_bar",4,'l');
          }
          t=t+1;
          goto me;
        } 
        digitalWrite(p[j],r);
        digitalWrite(p[k],s);
        digitalWrite(p[rst],1);
        if(num==7476)
          digitalWrite(p[set],1);
        if(detl==1)
        {
          my.dis_num(r,0,'c');
          my.dis_num(s,1,'c');
          my.dis_num(digitalRead(p[q]),3,'c');
          my.dis_num(digitalRead(p[qbar]),4,'c');
          delay(2000);
        }
        state_q=digitalRead(p[q]);
        state_qbar=digitalRead(p[qbar]);
        digitalWrite(p[clk],1);
        delay(2);
        if(detl==1)
        {
          my.dis_num(1,2,'c');
          my.dis_num(digitalRead(p[q]),3,'c');
          my.dis_num(digitalRead(p[qbar]),4,'c');
          delay(2000);
        }
        digitalWrite(p[clk],0);
        if(r==0 && s==0)
        {
          if((digitalRead(p[q])!=state_q) || (digitalRead(p[qbar])!=state_qbar))
            is_ok=0;
        }
        if(r==0 && s==1)
        {
          if((digitalRead(p[q])!=0) || (digitalRead(p[qbar])!=1))
            is_ok=0;
        }
        if(r==1 && s==0)
        {
          if((digitalRead(p[q])!=1) || (digitalRead(p[qbar])!=0))
            is_ok=0;
        }
        if(r==1 && s==1)
        {
          if((digitalRead(p[q])==state_q) || (digitalRead(p[qbar])==state_qbar))
            is_ok=0;
        }
        if(detl==1)
        {
          my.dis_num(0,2,'c');
          my.dis_num(digitalRead(p[q]),3,'c');
          my.dis_num(digitalRead(p[qbar]),4,'c');
          delay(3500);
        }
      }
      for(j=0;j<2;j++)
      {
        if(num==7473)
          j++;
        digitalWrite(p[rst],0);
        if(num==7476)
          digitalWrite(p[set],1);
        if((digitalRead(p[q])!=0)&&(digitalRead(p[qbar])!=1))
          is_ok=0;
        if(detl==1)
        {
          my.erase_row(5);
          my.string("-RESETTING-",5,'c');
          my.dis_num(digitalRead(p[q]),3,'c');
          my.dis_num(digitalRead(p[qbar]),4,'c');
          delay(2000);
        }
        if(num==7476)
        {
          digitalWrite(p[rst],1);
          digitalWrite(p[set],0);
          if((digitalRead(p[q])!=1)&&(digitalRead(p[qbar])!=0))
            is_ok=0;
          if(detl==1)
          {
            my.erase_row(5);
            my.string("-SETTING-",5,'c');
            my.dis_num(digitalRead(p[q]),3,'c');
            my.dis_num(digitalRead(p[qbar]),4,'c');
            delay(2000);
          }
        }
      }
      my.erase();
      my.string("FLIP FLOP",1,'c');
      my.dis_num(x,3,'c');
      if(is_ok==1)
        my.string("GOOD",5,'c');
      else
        my.string("BAD",5,'c');
      delay(2000);
      x++;
    }
}
//###############################################################################
int dec2bin(int n)
{
  int i=1,res=0,digit,a;
  while(n>0)
  {
    digit=n%2;
    res=res+(digit*i);   
    n=n/2;
    i=i*10;
  }
  return(res);
}
//###############################################################################
