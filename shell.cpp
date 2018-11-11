#include<iostream>
#include <string>
using namespace std;
#include<map>
#import<regex>
#include<cmath>
string command(""),valuename(""),value(""),symbol("+-*/><=!|&()"),num("+-*/"),logic("><=!|&"),functionname,number("0123456789");
bool err=false,isfunction;
int main_a=1,ii,iii,ceng=-1,x,base;
map<char,int> yxj;
map<int,string> condition;
map<int,int> lasta;
int con=-1;
regex pattern;
class data{
private:
	int len;
public:
	double d;
	bool b;
	string s;
	map<int,data> element;
	string type;
	int length(){
		return this->len;
	}
	void init(){
		this->type="Array";
		this->len=-1;
	}
	void add(data obj){
		this->len++;
		this->element[len]=obj;
	}
};
class Object{
private:
	int i;
public:
	map<string,data> element;
	map<string,int> index;
	map<int,string> namelist;
	void add(){
		this->i+=1;
	}
	int max(){
		return this->i;
	}
	void init(){
		this->i=-1;
	}
};
Object obj(){
	Object temp;
	temp.init();
	return temp;
}
Object global;
data jvm(string code,Object domain);
bool checkdomain(string str,Object valueobj){
	ii=0;
	while(ii<valueobj.max()+1){
		if(valueobj.namelist[ii]==str){
			return true;
		}
		ii++;
	};
	return false;
}
bool checkglobal(string str){
	ii=0;
	while(ii<global.max()+1){
		if(global.namelist[ii]==str){
			return true;
		}
		ii++;
	}
	return false;
}
bool isins(char s,string str){
	int ss=0;
	while(ss<str.length()){
		if(s==str[ss]){return true;}
		ss++;
	}
	return false;
}
data parseData(string key,Object area){
	data d_linshi;
	command="";
if(0x29<key[0]&&key[0]<0x40){
	d_linshi.d=stod(key);
	d_linshi.type="number";
	return d_linshi;
}
if(key[0]==0x22){
	ii=1;
	if(key[key.length()-1]!='\"'){
		err=true;
		cout<<main_a<<"：错误：字符串缺少'\"'\n";
		return data();
	}
	while(ii<key.length()){
		if(key[ii]==0x22){
			ii++;
			break;
		}
		command+=key[ii];
		ii++;
	}
	if(ii<key.length()){
		err=true;
		cout<<main_a<<"：错误：错误的变量值'"<<key<<"'\n";
		return data();
	}
	d_linshi.s=command;
	d_linshi.type="string";
}
else if(key=="true"){
	d_linshi.b=true;
	d_linshi.type="boolean";
}
							else if(key=="false"){
								d_linshi.b=false;
								d_linshi.type="boolean";
							}
							else if(key=="undefined"){
								d_linshi.type="undefined";
							}
							else if(key=="null"){
								d_linshi.type="null";
							}
							else{
								if(checkdomain(key,area)==true){
									return area.element[key];
								}
								else if(checkglobal(key)==true){
									return global.element[key];
								}
								else{
									if(key.find('(')!=-1){
										return jvm(key,area);
									}
									cout<<main_a<<"：错误：变量'"<<key<<"'未定义\n";
									err=true;
									return data();
								}
							}
							return d_linshi;
}
string getdata(data obj){
	if(obj.type=="string"){
		return obj.s;
	}
	if(obj.type=="number"){
		return to_string(obj.d);
	}
	if(obj.type=="boolean"){
		if(obj.b==false){
			return "false";
		}
		else{
			return "true";
		}
	}
	if(obj.type=="undefined"){
		return "undefined";
	}
	if(obj.type=="NaN"){
		return "NaN";
	}
	return "null";
}
data createfunction(string exp,string fcode){
	data temp;
	temp.init();
	temp.type="function";
	ii=0;
	exp+=",";
	command="";
	iii=0;
	while(ii<exp.length()){
		if(exp[ii]==','){
			if(command==""&&iii>0){
				err=true;
				cout<<main_a<<"：错误：形参不能为空\n";
				return temp;
			}
			data d_linshi;
			d_linshi.type="string";
			d_linshi.s=command;
			temp.add(d_linshi);
			iii+=1;
		}
		else{
			command+=exp[ii];
		}
		ii++;
	}
	temp.s=fcode;
	temp.d=iii;
	return temp;
}
void loadfunction(string name,string args,string fcode,Object *area){
	area->add();
	area->index[name]=area->max();
	area->namelist[area->max()]=name;
	area->element[name]=createfunction(args,fcode);
}
data calculate(string expression,Object area){
	data x1,x2;
	yxj['+']=4;
	yxj['-']=4;
	yxj['/']=5;
	yxj['%']=5;
	yxj['*']=5;
	yxj['(']=7;
	yxj[')']=0;
	yxj['&']=2;
	yxj['|']=1;
	yxj['!']=3;
	yxj['>']=3;
	yxj['<']=3;
	yxj['=']=3;
	command="";
	int cal=0,s=-1,l=-1,dui=-1;
	map<int,string> duilie;
	while(cal<expression.length()){
		if(isins(expression[cal],symbol)){
			if(expression[cal]=='('){
				command+='(';
				cal++;
				s=1;
				while(cal<expression.length()){
					if(expression[cal]=='('){
						s++;
					}
					else if(expression[cal]==')'){
						s--;
					}
					if(s==0){command+=')';break;}
					command+=expression[cal];
					cal++;
				}
				if(s>0){
					err=true;
					cout<<main_a<<"：错误：函数参数缺少括号\n";
					break;
				}
				dui++;
				duilie[dui]=command;
				command="";
				cal++;
				continue;
			}
			if(command!=""){
				dui++;
				duilie[dui]=command;
				command="";
			}
			dui++;
			duilie[dui]=expression[cal];
		}
		else{
			command+=expression[cal];
			functionname=expression[cal];
		}
		cal++;
	}
	if(err==true){
		return data();
	}
	if(command!=""){
		dui++;
		duilie[dui]=command;
		command="";
	}
	cal=-1;
	map<int,data> stack,list;
	while(cal<dui){
		cal++;
		if(isins(duilie[cal][0],symbol)){
			if(duilie[cal][0]=='('||s==-1){
				s++;
				data d_linshi;
				d_linshi.s=duilie[cal];
				d_linshi.type='o';
				stack[s]=d_linshi;
			}
			else if(duilie[cal][0]==')'){
				while(s>-1){
					if(stack[s].s[0]=='('){
						stack.erase(s);
						s--;
						break;
					}
					l++;
					list[l]=stack[s];
					stack.erase(s);
					s--;
				}
			}
			else if(yxj[duilie[cal][0]]>yxj[stack[s].s[0]]&&yxj[stack[s].s[0]]!='('){
				s++;
				data d_linshi;
				d_linshi.s=duilie[cal];
				d_linshi.type='o';
				stack[s]=d_linshi;
			}
			else{
				while(yxj[duilie[cal][0]]<=yxj[stack[s].s[0]]){
					if(stack[s].s[0]=='('){
						break;
					}
					l++;
					list[l]=stack[s];
					s--;
				}
				s++;
				data d_linshi;
				d_linshi.s=duilie[cal];
				d_linshi.type='o';
				stack[s]=d_linshi;
			}
		}
		else{
			l++;
			list[l]=parseData(duilie[cal],area);
		}
	}
	while(s>-1){
		l++;
		list[l]=stack[s];
		s--;
	}
	command="";
	cal=-1;
	while(cal<l){
		cal++;
		if(list[cal].type[0]=='o'){
			x2=stack[s];
			s--;
			x1=stack[s];
			if(isins(list[cal].s[0],num)){
				if(list[cal].s[0]=='+'){
					if(x1.type=="number"&&x2.type=="number"){
						x1.d+=x2.d;
					}
					else{
						x1.s+=x2.s;
					}
				}
				else{
				if(x1.type!="number"||x2.type!="number"){
					x1.type="NaN";
				}
				else if(list[cal].s[0]=='-'){
					x1.d-=x2.d;
				}
				else if(list[cal].s[0]=='*'){
					x1.d*=x2.d;
				}
				else if(list[cal].s[0]=='/'){
					x1.d/=x2.d;
				}
				}
			}
			else{
				if(x1.type=="undefined"){
					x1.type="null";
					x1.s="null";
				}
				if(x2.type=="undefined"){
					x2.type="null";
					x2.s="null";
				}
				if(x1.type=="NaN"||x2.type=="NaN"){
					x1.b=false;
				}
				else if(x1.type=="null"&&x2.type!="null"){
					x1.b=false;
				}
				else if(x1.type!="null"&&x2.type=="null"){
					x1.b=false;
				}
				else if(list[cal].s[0]=='>'){
					x1.b=x1.d>x2.d;
				}
				else if(list[cal].s[0]=='<'){
					x1.b=x1.d<x2.d;
				}
				else if(list[cal].s[0]=='='||'!'){
					if(x1.type=="number"){
						x1.s=to_string(x1.d);
					}
					if(x2.type=="number"){
						x2.s=to_string(x2.d);
					}
					if(x1.type=="boolean"){
						x1.s=to_string(x1.b);
					}
					if(x2.type=="boolean"){
						x2.s=to_string(x2.b);
					}
					if(list[cal].s[0]=='='){
						x1.b=x1.s==x2.s;
					}
					else{
						x1.b=x1.s!=x2.s;
					}
				}
				else{
					if(x1.type!="boolean"||x2.type!="boolean"){
						x1.b=false;
					}
					else if(list[cal].s[0]=='&'){
						x1.b=x1.b&&x2.b;
					}
					else if(list[cal].s[0]=='|'){
						x1.b=x1.b||x2.b;
					}
				}
				x1.type="boolean";
			}
			stack[s]=x1;
		}
		else{
			s++;
			stack[s]=list[cal];
		}
	}
	return stack[0];
}
data jvm(string code,Object domain){
	ceng++;
	data outdata;
	outdata.type="null";
	if(err==true){
		return outdata;
	}
	int i=0;
	while(i<code.length()){
		if(code[i]=='\n'){
			i++;
			main_a+=1;
			command="";
			continue;
		}
		else{
			if(command=="return"){
				if(code[i]=='\n'||i==code.length()-1){
					return outdata;
				}
				else{
					command="";
					i++;
					while(i<code.length()){
						if(code[i]=='\n'){
							break;
						}
						command+=code[i];
						i++;
					}
					outdata=calculate(command,domain);
					command="";
					if(err==true){
						return data();
					}
					else{
						return outdata;
					}
				}
			}
			else if(command=="break"){
				if(con==-1){
					err=true;
					cout<<main_a<<"：错误：break只能用于结束循环\n";
				}
				break;
			}
			else if(code[i]==' '){//空格
				if(command=="var"){
					i++;
					while(i<code.length()){//切分号
						if(code[i]=='='){//等号
							if(valuename==""){
								cout<<main_a<<"：错误：错误的变量名'"<<valuename<<"'\n";
								err=true;
								break;
							}
							value="";
							i++;
							while(i<code.length()){
								if(code[i]==','||code[i]=='\n'){break;}
								value+=code[i];
								i++;
							}
							if(value==""){
								cout<<main_a<<"：错误：请输入变量的值\n";
								err=true;
								break;
							}
							global.add();
							global.index[valuename]=global.max();
							global.namelist[global.max()]=valuename;
							data d_linshi=calculate(value,domain);
							if(err==true){
								break;
							}
							global.element[valuename]=d_linshi;
							valuename="";
						}
						else if(code[i]==','){
							i++;
							continue;
						}
						else{
							valuename+=code[i];
						}
						if(code[i]=='\n'){
							break;
						}
						i++;
					}
				}
				else if(command=="logout"){
					i++;
					command="";
					while(i<code.length()){
						if(code[i]=='\n'){
							break;
						}
						command+=code[i];
						i++;
					}
					data d_linshi=calculate(command,domain);
					if(err==true){break;}
					if(d_linshi.type=="number"){
						if(d_linshi.d-(double)floor(d_linshi.d)==0){
							command=to_string((int)(d_linshi.d));
						}
						else{
							command=to_string(d_linshi.d);
						}
					}
					else{
						command=getdata(d_linshi);
					}
					cout<<command<<"\n";
				}
				else if(command=="function"){
					i++;
					valuename="";
					while(i<code.length()){
						if(code[i]=='('){
							break;
						}
						else if(code[i]=='{'){
							err=true;
							cout<<main_a<<"：错误：函数'"<<valuename<<"'定义缺少参数()\n";
							break;
						}
						valuename+=code[i];
						i++;
					}
					if(err==true){break;}
					i++;
					ii=0;
					value="";
					while(i<code.length()){
						if(code[i]=='{'){
							err=true;
							cout<<main_a<<":错误：函数'"<<valuename<<"'的参数定义缺少')'\n";
							break;
						}
						if(code[i]==')'){
							break;
						}
						value+=code[i];
						i++;
					}
					if(err==true){break;}
					command="";
					i++;
					if(code[i]!='{'){
						err=true;
						cout<<main_a<<"错误：函数'"<<valuename<<"'的代码定义缺少'{'\n";
						break;
					}
					ii=1;
					i++;
					while(i<code.length()){
						if(code[i]=='{'){
							ii++;
						}
						else if(code[i]=='}'){
							ii--;
						}
						if(ii==0){
							break;
						}
						command+=code[i];
						i++;
					}
					if(ii>0){
						err=true;
						cout<<main_a<<"错误：函数'"<<valuename<<"'的代码定义缺少'}'\n";
						break;
					}
					if(ceng==0){
						loadfunction(valuename,value,command,&global);
					if(err==true){
						break;
					}
					}
					else{
						loadfunction(valuename,value,command,&domain);
						if(err==true){
							break;
						}
					}
					command="";
				}
				else if(command=="let"){
					i++;
					valuename="";
					while(i<code.length()){
						if(code[i]=='='){
							value="";
							i++;
							while(i<code.length()){
								if(code[i]==','||code[i]=='\n'){i-=1;break;}
								value+=code[i];
								i++;
							}
						}
						else if(code[i]==','||code[i]=='\n'||i==code.length()-1){
							if(value==""){
								err=true;
								cout<<main_a<<"：错误：请输入变量的值\n";
								break;
							}
							if(checkdomain(valuename,domain)==true){
								err=true;
								cout<<main_a<<"：错误：let不允许变量'"<<valuename<<"'被定义两次\n";
								break;
							}
							data d_linshi=calculate(value,domain);
							if(err==true){break;}
							if(d_linshi.type=="null"){
								err=true;
								cout<<main_a<<"：错误：变量'"<<value<<"'未定义\n";
								break;
							}
							domain.add();
							domain.index[valuename]=domain.max();
							domain.namelist[domain.max()]=valuename;
							domain.element[valuename]=d_linshi;
							valuename="";
							if(code[i]=='\n'){break;}
						}
						else{
							valuename+=code[i];
						}
						i++;
					}
					if(err==true){break;}
				}
				else{
					err=true;
					cout<<main_a<<"：错误：未知的指令'"<<command<<"'\n";
					break;
				}
				command="";
			}
			else{
				if(code[i]=='='){
					if(command==""){err=true;
						cout<<main_a<<"：错误：错误的变量名'"<<command<<"'\n";
						break;
					}
					i-=command.length();
					valuename="";
					while(i<code.length()){
						if(code[i]==','||code[i]=='\n'||i==code.length()-1){
							if(value==""){
								err=true;
								cout<<main_a<<"：错误：请输入变量的值\n";
								break;
							}
							data d_linshi=calculate(value,domain);
							if(err==true){break;}
							if(checkdomain(valuename,domain)==true){
								domain.element[valuename]=d_linshi;
							}
							else if(checkglobal(valuename)==false){
								global.add();
								global.index[valuename]=global.max();
								global.namelist[global.max()]=valuename;
								global.element[valuename]=d_linshi;
							}
							else{
								global.element[valuename]=d_linshi;
							}
							valuename="";
							if(code[i]=='\n'){break;}
						}
						else if(code[i]=='='){
							if(valuename==""){
								err=true;
								cout<<main_a<<"：错误：错误的变量名'"<<command<<"'\n";
								break;
							}
							value="";
							i++;
							while(i<code.length()){
								if(code[i]==','||code[i]=='\n'){i-=1;break;}
								value+=code[i];
								i++;
							}
						}
						else{
						valuename+=code[i];
						}
						i++;
					}
					if(err==true){break;}
					command="";
				}
			else if(code[i]=='('){
				functionname=command;
				data d_linshi=calculate(command,domain);
				if(err==true){break;}
				if(d_linshi.type!="function"){
					err=true;
					cout<<main_a<<"：错误：'"<<command<<"'不是一个函数\n";
					break;
				}
				command="";
				ii=1;
				value="";
				i++;
				if(d_linshi.d>0){
					while(i<code.length()){
						if(code[i]=='('){
							ii++;
						}
						else if(code[i]==')'){
							ii--;
						}
						if(ii==0){
							break;
						}
						value+=code[i];
						i++;
					}
					if(ii>0){
						err=true;
						cout<<main_a<<"：错误：函数参数缺少括号\n";
						break;
					}
				}
				if(functionname[0]=='f'||functionname[0]=='e'){
					int qiantao=1;
					i+=2;
					base=i;
					if(functionname[0]=='e'){
						con++;
						lasta[con]=base;
					}
					while(i<code.length()){
						if(code[i]=='{'){
							qiantao++;
						}
						else if(code[i]=='}'){
							qiantao--;
						}
						if(qiantao==0){
							break;
						}
						i++;
					}
					if(qiantao>0){
						err=true;
						cout<<main_a<<"：嵌套语句缺少'}'\n";
						return data();
					}
					valuename=value;
					if(functionname[0]=='e'){
					condition[con]=value;}
					data d_linshi=calculate(value,domain);
					if(err==true){
						break;
					}
					command="";
					if(d_linshi.b==true){
						i=base;
					}
				}
				else{
					ii=0,iii=0,x=0;
					value+=",";
					valuename="";
					if(d_linshi.d>0){
					while(iii<value.length()){
						if(value[iii]=='('){
							ii++;
							isfunction=true;
						}
						else if(value[iii]==')'){
							ii--;
						}
						else if(value[iii]==','&&isfunction==false){
							valuename+="let "+d_linshi.element[x].s+"="+command+"\n";
							command="";
							x++;
						}
						if(ii==0){
							isfunction=false;
						}
						command+=value[iii];
						iii++;
					}
					}
					command="";
					return jvm(valuename+d_linshi.s,domain);
				}
			}
			else{
				if(code[i]=='}'){
					if(condition[con]!=""){
						if(getdata(calculate(condition[con],domain))=="true"){
							i=lasta[con];
							if(code[i]=='\n'){
								i++;
							}
						}
						else{condition[con]="",con--;}
						continue;
					}
					i++;
					command="";
					continue;
				}
				command+=code[i];
			}
			}
		}
		i++;
	}
	return outdata;
}
int main () {
//————————————加载预置基础函数————————————————
loadfunction("if","exp","return exp",&global);
loadfunction("while","exp","return exp",&global);
command="";
//——————————————————————————————————————————
cout<<fixed;
string sss;
regex pattern;
//———————————————代码预处理——————————————————
cout<<"输入exit来退出\n";
while(true){
cout<<">>";
getline(cin,sss);
if(sss=="exit"){
	break;
}
if(sss=="help"){
	cout<<"———————————————帮助——————————————————\n";
	cout<<"logout 表达式 用于输出\n";
	cout<<"var 变量名 表达式 用于定义变量\n";
	cout<<"x=y 给变量赋值\n";
	cout<<"———————————————帮助——————————————————\n";
	sss="";
	cin.clear();
	continue;
}
sss+="\n";
jvm(sss,obj());
sss="";
cin.clear();
}
return 0;
}
