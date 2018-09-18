#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

unordered_map<int, int> M;

class Node {
public:
    int data, count;
    Node *parent, *next;    
    unordered_map<int, Node*> children;

    Node(int data, Node *parent) {
        this->data = data;
        this->count = 0;
        this->parent = parent;
        this->next = NULL;
        (this->children).clear();
    }
};


bool comp(const int &A, const int &B) {
    if(M[A] > M[B]) return true;
    if(M[A] < M[B]) return false;

    return A < B;
}

bool comp_head(const pair<int, int> &A, const pair<int, int> &B) {
    if(A.first > B.first) return true;
    if(A.first < B.first) return false;

    return A.second < B.second;
}

Node *root = new Node(-1, NULL);
map<int, Node*> head, tail;
int T;
float x;


void insert(vector<int> &set, Node *root) {
    // for(int i = 0; i < set.size(); i++) {
    //     cout << set[i] << " ";
    // }
    // cout << endl;
    int i = 0;
    while(i < set.size() && M[set[i]] * 100 >= x * T) {
        if(root->children.find(set[i]) == root->children.end()) {
            root->children[set[i]] = new Node(set[i], root);

            if(head.find(set[i]) == head.end()) {
                head[set[i]] = root->children[set[i]];
                tail[set[i]] = root->children[set[i]];
            } else {
                tail[set[i]]->next = root->children[set[i]];
                tail[set[i]] = tail[set[i]]->next;
            }
        }

        root = root->children[set[i]];
        root->count++;
        i++;
    }
    return;
}

void print(Node *root) {
    cout << root->data << " " << root->count << endl;
    for(unordered_map<int, Node*> :: iterator it = root->children.begin(); it !=root->children.end(); it++) {
        print(it->second);
    }
}

void printLinks() {
    for(map<int, Node*> :: iterator it = head.begin(); it != head.end(); it++) {
        Node *start = it->second;
        cout << it->first << " ";

        while(start != NULL) {
            cout << "(" << start->data << ", " << start->count << ") ";
            start = start->next;
        }
        cout << endl;
    }
}

// void printsets(vector<vector<int> > &lol, char* file)
// {
//     for(int i=0;i<lol.size();i++)
//     {
//         for(int j=0;j<lol[i].size();j++)
//         {
//             cout<<lol[i][j]<<" ";
//         }
//         cout<<endl;
//     }
//     //cout << endl << endl;
// }

vector<vector<int> > generate_fpsets(vector<vector<int> > &lol) {
    // for(int i=0;i<lol.size();i++)
    // {
    //  for(int j=0;j<lol[i].size();j++)
    //  {
    //      cout<<lol[i][j]<<" ";
    //  }
    //  cout<<"ended here"<<endl;
    // }
    // cout<<endl<<endl;

    if(lol.size()==0)return vector<vector<int> >();
    int sum = 0;
    for(int i=0;i<lol.size();i++)
    {
        if(lol[i].back() == 0)
        {
            lol.erase(lol.begin()+i);
            i--;
        }
        else
        {
            sum+=lol[i].back();
        }
    }

    // cout<<lol[0].size()<<" sum = "<<sum<<endl;
    if(sum * 100 >= x * T)
    {
        vector<vector<int> > result;
        for(int i=0;i<lol.size();i++)
        {
            lol[i].pop_back();
        }
        int iter = lol[0].size();
        for(int i=0;i<iter;i++)
        {
            vector<vector<int> > temp;
            temp.assign(lol.begin(),lol.end());
            vector<vector<int> > tempres = generate_fpsets(temp);
            // cout<<"tempres for element "<<iter+1<<" ka "<<i<<"th tempres hai"<<endl;
            // printsets(tempres);
            for(int k = 0;k<tempres.size();k++)
            {
                result.push_back(tempres[k]);
            }

            for(int j=0;j<lol.size();j++)
            {
                lol[j].pop_back();
            }
        }

        for(int i=0;i<result.size();i++)
        {
            result[i].push_back(iter+1);


        }
        result.push_back(vector<int>(1,iter+1));
        return result;


    }

    return vector<vector<int> >();

}

vector<vector<int> > gen_freq() {
    vector<vector<int> > freq_sets;
    vector<int> freq_vec(head.size());
    vector<pair<int, int> > MC(head.size());
    int i = 0;
    for(map<int, Node*> :: iterator it = head.begin(); it != head.end(); it++) {
    	// cout << i << " " << it->first << endl;
        MC[i++] = make_pair(M[it->first], it->first);
    }

    // cout << "Endl\n";
    
    sort(MC.begin(), MC.end(), comp_head);

    for(int i = 0; i < MC.size(); i++) {
        freq_vec[i] = MC[i].second;

        // cout << i << " " << MC[i].second << endl;
    }

    unordered_map<int, int> rev_map;
    for(i = 0; i < freq_vec.size(); i++) {
        rev_map[freq_vec[i]] = i;

        // cout << "M " << rev_map[freq_vec[i]] << " " << i << endl;
    }

    i = 0;
    for(; i < freq_vec.size(); i++) {
        vector<vector<int> > lol;
        Node * start = head[freq_vec[freq_vec.size() - 1 - i]];
        // cout << freq_vec[freq_vec.size() - 1 - i] << " hua\n";
        while(start != NULL) {
            int freq = start->count;
            vector<int> set(head.size() - i, 0);

            Node *parent = start;
            // cout << "path "; 
            while(parent->data >= 0) {
                // cout << "(" << parent->data << ", " << rev_map[parent->data] << ") ";
                set[rev_map[parent->data]] = freq;
                parent = parent->parent;
            }
            // cout << endl;
            lol.push_back(set);
            start = start->next;
        }

        // printsets(lol);

        vector<vector<int> > ans = generate_fpsets(lol);

        // printsets(ans);
        for(int j = 0; j < ans.size(); j++) {
            for(int k = 0; k < ans[j].size(); k++) {
                ans[j][k] = freq_vec[ans[j][k] - 1];
            }

            freq_sets.push_back(ans[j]);
        }

        // printsets(ans);
    }

    return freq_sets;
}


int main(int argc, char *argv[]) {
    clock_t start, end;
    start = clock();
    string line;
    ifstream fin;
    fin.open(argv[1], ifstream::in);
    M.clear();
    T = 0;
    while (getline(fin, line)) {
        istringstream is(line);
        vector<int> set = vector<int>(istream_iterator<int>(is), istream_iterator<int>());
        for(int i = 0; i < set.size(); i++) {
            if(M.find(set[i]) == M.end()) {
                M[set[i]] = 1;
            } else {
                M[set[i]]++;
            }
        }
        T++;
    }
    fin.close();

    head.clear(); tail.clear();
    x = atoi(argv[3]);
    fin.open("retail.dat", ifstream::in);

    // cout << x * T << endl;
    while (getline(fin, line)) {
        istringstream is(line);
        vector<int> set = vector<int>(istream_iterator<int>(is), istream_iterator<int>());
        sort(set.begin(), set.end(), comp);
        insert(set, root);
        // print(root);
        // cout << "-------------------\n\n";
    }
    fin.close();

    vector<vector<int> >freq_sets = gen_freq();

    ofstream writeout;
	writeout.open(argv[2]);
	for(int i = 0; i < freq_sets.size(); i++) {
		for(int j = 0; j < freq_sets[i].size() - 1; j++) {
			writeout << freq_sets[i][j] << " ";
		}
        writeout << freq_sets[i][freq_sets[i].size() - 1] << endl;
	}
	writeout.close();

    // cout << "Size " <<  freq_sets.size() << endl;

    // printLinks();
    // print(root);
    end = clock();
    cout << (float) (end - start) / CLOCKS_PER_SEC << endl;

    return 0;
}