/**/
#include<iostream>
#include<iomanip>
#include<fstream>
#include<queue>
#include<cstring>
#include<vector>
using namespace std;

struct MinHeapNode{
    int chd;
    int freq;
    MinHeapNode *left, *right;
    MinHeapNode(int chd, int freq){
        left = right = NULL;
        this->chd = chd;
        this->freq = freq;
    }
};
struct compare{
    bool operator()(MinHeapNode* l,MinHeapNode* r){
        return(l->freq > r->freq);
    }
};

vector<int> Huffcode;
vector< pair<char,string> > charactermap; //for frequencies
priority_queue< MinHeapNode*, vector<MinHeapNode*>, compare> minHeap; //to create tree
class compress{
private:
    int freq[256],ascVal[256],tfreq[256];
    int freqCount=0,arrFreqCount=0,num=254;
    string code;
    char chh = char(num);
public:
    void init();
    void calFreq(char fname[]);
    void makeArrFreq();
    void printFrequencyTable();
    void createTree();
    void printBinary(struct MinHeapNode* root, string chr);
    void Convert_StringBits_to_IntegerBits();
    void save();
};
class bitwrite{
private:
    int bit;
    char buf;
    ostream& out;
public:
    bitwrite(ostream& os) : out(os),buf(0),bit(0){}//clear buffer and bit count
    void flush();
    void writeBit(int x);
};
class bitread{
private:
    char buf;
    int nbit;
    istream& in;
public:
    bitread(istream& is);
    void fill(); //for buffer
    int readbit(); //will scan the buffer to return 1 or 0
};
class decompress{
private:
    class node{
    public:
        char data;
        node *left, *right;
        node(char item){
            data = item;
            left = right = 0;
        }
    };
    typedef node* nodeptr;
public:
    decompress();
    void decodeTree(ifstream & f2p);
    void insert(char chin, string dcodee);
    char decodeChar(bitread& in);
    private:
    nodeptr rootd;
};
inline decompress::decompress(){
    rootd = new node('*');
}



void compress::init(){
    for(int i=0;i<255;i++){
        ascVal[i] = 0; //ASCII_values[i] = 0;
        tfreq[i] = 0; //actual_frequency[i] = 0;
        freq[i] = 0; //frequency[i] = 0;
    }
}
void compress::calFreq(char fname[]){
    int x,y,z;
    char calCh1,calCh2;
    fstream fp;
    fp.open(fname,ios::in);
    if(!fp){
        cout << "File Error"<<endl; system("pause"); exit(0);
    }else{
        while(fp.get(calCh1)){
            charactermap.push_back(make_pair(calCh1,""));
            freqCount++;
            y = int (calCh1);
            freq[y]++;
        }
        calCh2 = char(129);
        charactermap.push_back(make_pair(calCh2,""));
        freqCount++;
        fp.close();
    }
}
void compress::makeArrFreq(){
    int x=0, y=0;
    for(int i=0; i<254; i++){ //to filter only the occuring chars
        if(freq[i]!=0){
            tfreq[x] = freq[i];
            ascVal[y] = i;
            x++; y++; arrFreqCount++;
        }
    }
    tfreq[y] = 1;
    ascVal[x] = 129;
    arrFreqCount++;
}
void compress::printFrequencyTable(){
    char ch;
    string spacee = "", newLine = "";
    spacee = spacee + "Space";
    newLine = newLine + "New Line";
    cout<<"\n\tFrequency Table of Characters present in file\n\n";
	cout<<"\t===================================================\n";
	cout<<"\t  Ascii\tCharacters\t\tFrequency\n";
	cout<<"\t===================================================\n";
	for(int i=0;i<arrFreqCount; i++){
		ch = char(ascVal[i]);
		if(ch == ' '){
            cout<<"\t   "<<ascVal[i]<<"\t    "<<spacee<<"\t\t  "<<tfreq[i]<<"\n";
		}else if(ch == '\n'){
            cout<<"\t   "<<ascVal[i]<<"\t    "<<newLine<<"\t\t  "<<tfreq[i]<<"\n";
		}else{
            cout<<"\t   "<<ascVal[i]<<"\t    "<<ch<<"\t\t\t  "<<tfreq[i]<<"\n";
		}
	}
}
void compress::printBinary(struct MinHeapNode* root, string chr){
    if(!root){
        return;
    }if(root->chd != chh){
        char ch;
        ch = char(root->chd);
        cout<<"\t   "<<ch<<"\t\t\t"<<chr<<"\n";
        if(root->chd == ' '){
            code = code + "_" + " " + chr + "$";
        }else if(root->chd == '\n'){
            code = code + "newline" + " " + chr + "$"; //palitan yung newline na char
        }else{
            code = code + ch + " " + chr + "$";
        }
        for(int x=0; x<freqCount; x++){
            if(charactermap[x].first==ch){
                charactermap[x].second = chr;
            }
        }
    }
    printBinary(root->left, chr + "0");
    printBinary(root->right, chr + "1");
}
void compress::createTree(){
    struct MinHeapNode *left, *right, *top;
    for(int x=0; x<arrFreqCount; ++x){
        minHeap.push(new MinHeapNode(ascVal[x],tfreq[x]));
    }
    while(minHeap.size()!=1){
        left = minHeap.top(); //will take two least freq then remove the node
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        //create new node with the sum of two least freq then put it to he node
        top = new MinHeapNode(chh, left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    printBinary(minHeap.top(),"");
}
void compress::Convert_StringBits_to_IntegerBits(){
    string temp;
    for(int x=0; x<charactermap.size(); x++){
        temp = charactermap[x].second;
        for(int y=0; y<temp.size(); y++){ //storing each char to int vector
            if(temp[y]=='1'){
                Huffcode.push_back(1);
            }else{
                Huffcode.push_back(0);
            }
        }
    }
    Huffcode.push_back(-1);
}
void compress::save(){ //creates codes file
    fstream fp;
    fp.open("codes.txt",ios::out);
    for(int i=0; i<code.size(); i++){
        if(code[i]!='$'){
            fp<<code[i];
        }else{
            fp<<endl;
        }
    }fp.close();
}
void bitwrite::flush(){
    out.put(buf);
    out.flush();
    buf = bit = 0;
}
void bitwrite::writeBit(int x){
    if(x==-1){ //for last bit if buffer is not full still flush buffer
        flush();
    }if(bit == 8){ //if buffer full(8 bit) clear buffer
        flush();
    }
    unsigned char mask = 1;
    if(x == 1){
        mask = mask << (7-bit);
        buf = buf | mask;
    }if(x == 0){
        mask = mask << (7-bit);
        mask = ~mask;
        buf = buf & mask;
    }bit++;
}
void decompress::insert(char chin, string dcodee){
    decompress::nodeptr p = rootd;
    for(int x=0; x<dcodee.length(); x++){
        switch(dcodee[x]){
            case '0': if(p->left == 0) //create a tree
                        p->left = new decompress::node('*');
                        p = p->left;
                        break;
            case '1': if(p->right == 0)
                        p->right = new decompress::node('*');
                        p = p->right;
                        break;
            default: cout<< "Error in Code."<<endl; exit(0);
        }
    } p->data = chin;
}
void decompress::decodeTree(ifstream & f2p){
    char ch;
    string dcode;
    for(;;){
        if(f2p.eof())return;
        f2p >>ch>>dcode;
        cout<<"\n"<<ch<<" = "<<dcode;
        if(ch=="_"){
            ch = ' ';
        }if(ch=="newline"){ //palitan yung newline na char kase string sya e
            ch = '\n';
        }insert(ch,dcode);
    }
}
bitread::bitread(istream& is): in(is){ //initialize the value of buf and nbit
    buf = 0;
    nbit = 8;
}
void bitread::fill(){
    buf = in.get();
    nbit = 0;
}
int bitread::readbit(){
    if(nbit==8){
        fill();
    }
    unsigned char mask =1; //take the bit's value
    mask = mask << (7-nbit);
    mask = mask & buf;
    nbit++;
    if(mask == 0){
        return 0;
    }else{
        return 1;
    }
}
char decompress::decodeChar(bitread& in){
    decompress::nodeptr p;
    p = rootd;
    int bitt;
    while(true){
        bitt = in.readbit();
        if(bitt == 1){
            p = p->right;
        }if(bitt == 0){
            p = p->left;
        }if(p->right==NULL || p->left==NULL){
            break;
        }
    }return(p->data);
}



int menu(){
    int choice;
    printf("\n\n\t\t[1] Encode\n");
    printf("\t\t[2] Decode\n");
    printf("\t\t[3] Exit\n");
    printf("\n\t\tInput choice: ");
    scanf("%d",&choice);
    return choice;
}
int main(){
    compress obj;
	obj.init();
	int decompressBit, currBit = 0;
    char data, filename[50];
    string decode;
	while(1){
        system("cls");
        cout<<"\n\t=====================================";
        cout<<"\n\t Text File Compressor & Decompressor\n";
        cout<<"\t=====================================";
        switch(menu()){
            case 1:
            {
                   cout<<"\n\nInput file to compress (.txt): ";
                   cin>>filename;

                   system("cls");
                   obj.calFreq(filename);   	//To calculate frequency of each character in file
                   obj.makeArrFreq();   //Make actual frequency array and ASCII values
                   obj.printFrequencyTable();  // Print frequency table
                   system("pause");

                   system("cls");
	               cout<<"\nCharacter Table with Huffman codes \n";
	               cout<<"\n\tCharacter\t\tHuffman Code\n";
                   obj.createTree();   //Function to build Huffman tree
                   obj.Convert_StringBits_to_IntegerBits();          //Converts character from charactermap to int vector

                   int len = strlen(filename); //will put .cmp extension on file
	               filename[len-1] = 'p';
	               filename[len-2] = 'm';
	               filename[len-3] = 'c';
	               cout<<"\n";
	               obj.save();
	               int tempp;
	               ofstream fp;
	               fp.open(filename,ios::binary);
	               bitwrite s(fp);
	               for(int x=0; x<Huffcode.size(); x++){ //compressed file
                        tempp = Huffcode[x];
                        s.writeBit(tempp);
	               }
	               cout<<"\nCompression success.\n"; fp.close();
	               system("pause");
	               break;
            }
            case 2:
            {
                   cout <<"\n\nInput code file(.txt): ";
                   cin >> filename;
                   ifstream fps(filename);
                   if(!fps.is_open()){
                         cout << "File Error"<<endl; system("pause"); exit(0);
                   }else{
                        decompress dobj;
                        dobj.decodeTree(fps);
                        cout << "\nInput the compressed file(.cmp): ";
                        cin >> filename;
                        ifstream in;
                        ofstream fout;
                        fout.open("Decompressed.txt"); //creating decompressed file and put decoded text
                        in.open(filename,ios::binary);
                        bitread os(in);
                        if(!in.is_open()){
                               cout << "File Error"<<endl; system("pause"); exit(0);
                        }
                        char chc = char(129);
                        while(true){
                            data = dobj.decodeChar(os);
                            if(data == chc){
                                break;
                            }currBit++;
                            cout<<data;
                        }
                        cout<<"\nDecompression Successful !!!\n";
                        system("pause");
                   }break;
            }
            case 3:exit(0);
        }
    }
    return 0;
}
