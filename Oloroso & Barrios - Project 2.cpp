#include<cstring>
#include<vector>
#include<iostream>
#include<iomanip>
#include<queue>
#include<fstream>
using namespace std;

string str,codes,output;
int q = 0;
int count3 = 0;
int count1 = 0;

int num = 254,ct = 0;
char chh = char(num);
vector<pair<char,string> > charactermap;
vector<int>Huffcode;

int frequency[256], ASCII_values[256], actual_frequency[256];

//================================================================================================
class BitwiseWrite{
	private:
	  char buf;
	  int Tbits;
	  ostream& out;
	public:


  BitwiseWrite(ostream & os) : out(os),buf(0),Tbits(0)
  {
 							//clear buffer and bit counter
  }
  void flush();   	 // write buffer into output file and clear it
  void writeBit(int i);
};

void BitwiseWrite::flush() {
  out.put(buf);
  out.flush();
  buf = Tbits = 0;
}

void BitwiseWrite::writeBit(int i) // Write the least significant bit of the argument to the bit buffer
{
	if( i == -1)     //for last bit if buffer is not full still flush buffer
	{
		flush();
	}
  if(Tbits == 8) //if buffer full(8 bit) clear buffer
  {
    flush();
  }

  //if the int is 1, we must put the bit 1 into the buffer
  unsigned char mask = 1;
  if(i == 1)
  {
    mask = mask << (7 - Tbits);
    buf = buf | mask;
  }
  //if the int is 0, we must put the bit 0 into the buffer
  if(i == 0)
  {
    mask = mask << (7 - Tbits);
    mask = ~mask;
    buf = buf & mask;
  }
  Tbits++;                   //increment Tbits++
}
//===================================================================================================

class BitwiseRead {
private:
  char buf;
  //int first;
  int nbits;        //how many bits have been read from the buffer
  istream& in;
public:

  /* Initialize a BitwiseRead that will use the given istream for input
 * */
  BitwiseRead(istream & is);

  /* Fill the buffer from the input */
  void fill();

  /* Read the next bit from the bit buffer. Fill the buffer form the input
 * stream first if needed.
 * Return 1 if the bit read is 1, 0 if bit read is 0
 * */
  int readBit();
};

BitwiseRead::BitwiseRead(istream & is) : in(is) {
    buf = 0;
    nbits = 8;
}



void BitwiseRead::fill()
{
	buf = in.get();
	nbits = 0;
}

int BitwiseRead::readBit()
{
  if(nbits == 8) {
    fill();
 }

  //uses a mask to extract the nbits'th bit
  unsigned char mask = 1;
  mask = mask << (7-nbits);
  mask = mask & buf;
  nbits++;
  if(mask == 0) {
    return 0;
  }
  else {
    return 1;
  }
}

struct MinHeapNode{
    int data;
    int freq;
    MinHeapNode *l, *r;

    MinHeapNode(int data, int freq){

        l = r = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// For comparison of
// two heap nodes (needed in min heap)
struct compare {

    bool operator()(MinHeapNode* l, MinHeapNode* r)

    {
        return (l->freq > r->freq);
    }
};

// Prints huffman codes from
// the root of Huffman Tree.
int p;
string str1= "";
int n=0;


void printCd(struct MinHeapNode* root, string str){
    int i;
    if (!root)
        return;

    if (root->data != chh)
    {
		char ch;
		ch = char(root->data);
        cout <<"\t    "<< ch << "\t\t\t  " << str << "\n";
	if(root->data == ' ')
		codes = codes + "_" + " " + str + "$";
	else
		codes = codes + ch + " " + str + "$";

	for(i=0;i<count3;i++){
		if(charactermap[i].first == ch){
			charactermap[i].second = str;
            }
        }
    }
    printCd(root->l, str + "0");
    printCd(root->r, str + "1");
}
// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
void HuffmanCodes(int data[], int freq[], int size){
    struct MinHeapNode *l, *r, *top;

    // Create a min heap & inserts all characters of data[]

    for (int i = 0; i < size; ++i)                                //Build Heap
        minHeap.push(new MinHeapNode(data[i], freq[i]));

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1) {

        // Extract the two minimum
        // freq items from min heap
        l = minHeap.top();
        minHeap.pop();

        r = minHeap.top();
        minHeap.pop();

        // Create a new internal node with
        // frequency equal to the sum of the
        // two nodes frequencies. Make the
        // two extracted node as left and right children
        // of this new node. Add this node
        // to the min heap 'â– ' is a special value
        // for internal nodes, not used
        top = new MinHeapNode(chh, l->freq + r->freq);

        top->l = l;
        top->r = r;
        minHeap.push(top);
    }
    // Print Huffman codes using
    // the Huffman tree built above
    printCd(minHeap.top(),"");
}
int total_bits_written()
{
	string temp;
	int total_bits = 0;
	for(int i=0; i < charactermap.size();i++)
	{
		temp = charactermap[i].second;
		for(int j=0; j < temp.length();j++)
		{
			total_bits++;
		}
	}
	return total_bits;
}

void calculate_frequency(int freq[],char filename[]){
    char ch,ch1;
    int x,y,f;
    ifstream fin;
    fin.open(filename);
	if(!fin.is_open()){
		cout <<"Error !!! Cannot open Source file.\n";
		cin>>y;
		exit(1);
	}
    while(fin.get(ch)){
        if(ch>=32&&ch<255){
            charactermap.push_back(make_pair(ch,""));
            count3++;
        }
        y = int(ch);
        freq[y]=freq[y]+1;
    }
    ch1 = char(129);
    charactermap.push_back(make_pair(ch1,""));
    count3++;
    fin.close();
}

void save(){    //creates codes file
	ofstream out;
	out.open("codes.txt");
	for(int i=0;i<codes.size();i++){
		if(codes[i] != '$')
			out<<codes[i];
		else
			out<<endl;
	}
	out.close();
}

void SeperateCharacter(int freq[],int actual_frequency[],int ASCII_values[]){
	 int k =0;
   	 int y=0;
   	 for(int j=32;j < 254;j++){   //For seperating occurring and non-occurring characters & to create actual
		if(freq[j] != 0){
            actual_frequency[k] = freq[j];
           	 ASCII_values[y] = j;
           	 y++;
           	 k++;
           	 count1++;
		}
    }
	actual_frequency[y] = 1;     // For adding pseudo-EOF in actual_frequency & ASCII_values arrays
	ASCII_values[k] = 129;
	count1++;
}

void Convert_StringBits_to_IntegerBits(){
	string temp;
	for (int i = 0; i < charactermap.size();i++){
		temp = charactermap[i].second;
		for(int j=0;j < temp.size();j++){          //For storing strings bits into integer vector,for 								   writing bit by bit in file
			if(temp[j] == '1')
				Huffcode.push_back(1);
			else
				Huffcode.push_back(0);
		}
     		//cout  <<"\t"<< charactermap[i].first <<"\t"<< charactermap[i].second<<"\n";
  	  }
	Huffcode.push_back(-1);
}
void printFrequencyTable(int actual_frequency[],int ASCII_values[]){
	char ch;
	string str = "";
	str = str+"Space";
	cout<<"\n\tFrequency Table of Characters present in file\n\n";
	cout<<"\t===================================================\n";
	cout<<"\tCharacters\t\tFrequency\n";
	cout<<"\t===================================================\n";
	for(int i=0;i<count1; i++){
		ch = char(ASCII_values[i]);
		if(ch == ' ')
			cout<<"\t    "<<str<<"\t\t  "<<actual_frequency[i]<<"\n";
		else
			cout<<"\t    "<<ch<<"\t\t\t  "<<actual_frequency[i]<<"\n";
	}
}

class Huffman
{
	private:
	/*** Node structure ***/
	class node
	{
		public:
		char data;
		node *l, *r;

		node(char item)   // node constructor
		{
			data = item;
			l = r = NULL;
		}
	};
	typedef node * nodePointer;
	public:
		Huffman();
		void insert(char ch, string code);
		char decode(BitwiseRead &in);

		private:
		nodePointer root;
	};
inline Huffman::Huffman(){
	root = new node('*');
}
//======================================================================================
void Huffman::insert(char ch, string code)          //traverses each character from file from its huffman code
{
	Huffman::nodePointer p = root;
	for(int i = 0; i < code.length(); i++)
	{
		switch (code[i])
		{
		case '0' :
				if (p->l == 0) // create node along path
				p->l = new Huffman::node('*');
				p = p->l;
				break;
		case '1' :
				if (p->r == 0) // create node along path
				p->r = new Huffman::node('*');
				p = p->r;
				break;
		default:
			cerr << "*** Illegal character in code ***\n";
			exit(1);
		}
	}
	p->data = ch;
}


char Huffman::decode(BitwiseRead &in){             //To decode from huffman tree
	Huffman::nodePointer p;
	p = root;
	int bit;
	while(true){
		bit = in.readBit();
		//count++;
		if(bit == 1)
			p = p -> r;
		if(bit == 0)
			p = p -> l;
		if(p->r == NULL || p->l == NULL)
			break;
	}
	return (p->data);
}

void init(){
for(int i=0;i<255;i++){
        ASCII_values[i] = 0;
        actual_frequency[i] = 0;
        frequency[i] = 0;
    }
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
	char ch;
	char filename[50];

	string decode;
	init();

    ifstream in;
    ofstream fout;
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
                   calculate_frequency(frequency,filename);  	//To calculate frequency of each character in file
                   SeperateCharacter(frequency,actual_frequency,ASCII_values);  //Make actual frequency array and ASCII values
                   printFrequencyTable(actual_frequency,ASCII_values);  // Print frequency table
                   system("pause");

                   system("cls");
	               cout<<"\nCharacter Table with Huffman codes \n";
	               cout<<"\n\tCharacter\t\tHuffman Code\n";
                   HuffmanCodes(ASCII_values,actual_frequency,count1);  //Function to build Huffman tree
                   Convert_StringBits_to_IntegerBits();          //Converts character from charactermap to int vector
                   int len = strlen(filename);
	               filename[len-1] = 'p';
	               filename[len-2] = 'm';
	               filename[len-3] = 'c';
	               cout<<"\n";
	               save();
	               BitwiseWrite s(fout);
	               cout<<"\nCompression success.\n";
	               system("pause");
	               break;
            }
            case 2:
            {
                   cout <<"\n\nInput code file(.txt): ";
                   cin >> filename;
                   ifstream codestream(filename);
                   if (!codestream.is_open()){
                        cout <<"Error !!! Cannot open code file.\n";
                        exit(1);
                   }
                   Huffman h;
                   cout << "\nInput the compressed file(.cmp): ";
                   cin >> filename;
                   fout.open("Decompressed.txt");
                   in.open(filename,ios::binary);
                   BitwiseRead os(in);
                   if(!in.is_open()){
                       cout << "Error !!! Cannot open Compressed file.\n";
                       exit(1);
                   }
                   cout<<"\nDecompression Successful !!!\n";
                   system("pause");
                   break;
            }
            case 3:exit(0);
        }
    }
    return 0;
}
