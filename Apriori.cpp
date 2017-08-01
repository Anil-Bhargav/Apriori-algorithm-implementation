#include <bits/stdc++.h>
#define threshold 1
using namespace std;

typedef set<int> item_set;
typedef map<item_set,int> cand_set;
typedef map<int,cand_set> L_table;

L_table result;

void get_dbase(fstream &d,vector<set<int>> &dbase){
	//Takes the contents of file into local data_structure;
	
	while(1){
		string tr="";
		getline(d,tr);
		item_set trans;
		
		for(int i=0;i<tr.length();i++){
			string pre_num="";
			if(tr[i]=='I'){
				i++;
				while(i<tr.length()&&tr[i]!=','){
				pre_num+=tr[i];
				i++;	
				}
				trans.insert(atoi(pre_num.c_str()));
			}
		}
		
		dbase.push_back(trans);
		if(d.eof())break;
	}
}

cand_set get_L1(vector<item_set> &dbase){
	//Generates L1 from raw data;
	
	cand_set L1;
	item_set :: iterator it;
	for(int i=0;i<dbase.size();i++){
		for(it=dbase[i].begin();it!=dbase[i].end();it++){
			item_set temp;
			temp.insert(*it);
			if(L1.find(temp)==L1.end())
			L1[temp]=1;
			else L1[temp]++;
		}
	}
	cand_set :: iterator it1;
	it1=L1.begin();
	
	while(it1!=L1.end()){
	    	if(it1->second<threshold){
	        cand_set :: iterator d=it1;
	        it1++;
		    L1.erase(d);	
		}
		else it1++;
	}
return L1;
} 

bool has_infreq(item_set &res,cand_set &L,int k){
	//Checks if the set res has infreq subset of len k-1 in present in Lk;
	
	item_set :: iterator ite1=res.begin();
	vector<int> check;
	for(;ite1!=res.end();ite1++)
	check.push_back(*ite1);
	set<int> check1;
	for(int i=0;i<k;i++)
	check1.insert(check[i]);
	
	int n=check.size();
	
	for(int j=k,cnt=0;cnt<n-1;j++,cnt++){
		if(L.find(check1)==L.end())
		return true;
		check1.erase(check1.begin());
		check1.insert(check[j%n]);
	}
	return false;
}

cand_set cand_gen(cand_set &L,int k){
	
	//Simulation of Candidate Generation;
	cand_set C;
	cand_set :: iterator it1,it2;
	for(it1=L.begin();it1!=L.end();it1++){
		item_set i1=it1->first;
		
		for(it2=L.begin();it2!=L.end();it2++){
			item_set i2=it2->first;
			item_set res;
			item_set :: iterator ite1=i1.begin(),ite2=i2.begin();
			if(k>1){
				int flg=0;
				for(int i=0;i<k-1;i++,ite1++,ite2++){
					if(*ite1!=*ite2){
						flg=1;break;
					}
					res.insert(*ite1);
				}
					if(flg)continue;
					else{
						if(*ite1<*ite2){
							res.insert(*ite1);
							res.insert(*ite2);
						}
						else continue;
					}
			}
			else{
				if(*ite1<*ite2){
					res.insert(*ite1);
					res.insert(*ite2);
			    }
			    else continue;
			}
			if(has_infreq(res,L,k))
			continue;
			else C[res]=0;
		}
	}
	return C;
}

bool contains(item_set &check,item_set &in){
	
	//Checks whether all elements of set 'check' are present in set 'in';
	item_set :: iterator it;
	for(it=check.begin();it!=check.end();it++){
		if(in.find(*it)==in.end())
		return false;
	}
	return true;
}

int main(){
	// File with the transaction data;
	fstream ipt_file;
	ipt_file.open("Database.txt");
	
	// Taking transaction sets into local storage, for easy manipulation
	vector<item_set> dbase;
	get_dbase(ipt_file,dbase);
	
	//get_L1 from the raw data
	cand_set L1=get_L1(dbase);
	cand_set :: iterator it;
	//store in the L_table
	result[1]=L1;
	
	//iterating for successive Li
	
	for(int i=2;!result[i-1].empty();i++){
		// Get Ci
		cand_set C=cand_gen(result[i-1],i-1);
		
		// Count occurences or each set in Ci in the main data;
	    for(it=C.begin();it!=C.end();it++){
	    	item_set check=it->first;
	    	for(int i=0;i<dbase.size();i++){
	    		if(contains(check,dbase[i]))
	    		C[check]++;
	        }
	    }
	    
	    //Deleting those sets which don't satisfy minimum support
	    it=C.begin();
	    while(it!=C.end()){
	    	if(it->second<threshold){
	        cand_set :: iterator d=it;
	        it++;
		    C.erase(d);	
		}
		else it++;
		}
        
        //Printing Li
	    for(it=C.begin();it!=C.end();it++){
		item_set :: iterator it1;
		for(it1=it->first.begin();it1!=it->first.end();it1++)
			cout<<"I"<<*it1<<",";
		cout<<"----"<<it->second<<endl;
	    }
	    //Storing Li
	    result[i]=C;
		cout<<endl<<endl;
	}
	return 0;
}
