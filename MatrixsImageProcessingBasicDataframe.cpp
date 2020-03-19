//Temel matris işlemlerini yapabilecek, temel görüntü işleme fonksiyonlarinin calistigi,
//ayni zamanda tablolama yapabilecek siniflarin olusturulmasi isteniyor.
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <math.h>

using namespace std;

//--------------------------------Matrix TEMPLATE sınıfı---------------------------
template <class V>
class Matrix{
protected:
    V rows,columns,value;// rows:matris satır boyutu, columns:matris sütun boyutu, value:matris çeşidi
    V **matris;//matrisi tutan dinamik dizi(pointer)
    void matrisCreate(int,int,V);//matris oluşturma fonksiyonu ✔
    void matrisDelete(); // matris silme fonksiyonu ✔
public:
    Matrix();//default constructer ✔
    Matrix(int,int,V);//constructer ✔
    Matrix(int,int,char);//constructer ✔
    virtual ~Matrix();//Destructer ✔

    void resize(int,int); //Matrisi yeniden boyutlandırma ✔
    virtual void print() const; //Matrisi ekrana yazdırma ✔
    void print(string) const; //Matrisi dosyaya yazdırma ✔
    Matrix<V>& T(); //Matrisin transpozu ✔
    Matrix<V>& emul(const Matrix&); // Matris çarpma
    Matrix<V>& inv(); //Matrisin tersi
    double det(); //Matrisin determinantı;

    Matrix<V>& operator+(const Matrix<V>&);//Matris + Matris ✔
    Matrix<V>& operator-(const Matrix<V>&);//Matris - Matris ✔
    Matrix<V>& operator*(const Matrix<V>&);//Matris * Matris ✔
    Matrix<V>& operator+(V);//Matris + Scaler ✔
    Matrix<V>& operator-(V);//Matris - Scaler ✔
    Matrix<V>& operator*(V);//Matris * Scaler ✔
    Matrix<V>& operator/(V);//Matris / Scaler ✔
    Matrix<V>& operator%(V);//Matris % Scaler ✔ (kalan)
    Matrix<V>& operator^(V);//Matris ^ Scaler ✔ (üs alma)

};

//----------------------Matris Oluşturma------------------------------------------
template<typename V>
void Matrix<V>::matrisCreate(int rows, int columns, V value){
    this->rows = rows;//gelen degerleri matrisin rows ve columns değişkelerine eşitliyorum.
    this->columns = columns;
    matris = new V*[rows];
    srand(time(0));
    for (int i = 0; i < rows; i++)//belirtilen matris kadar bellekten yer ayırıyorum.
    {
        matris[i] = new V[columns];
    }

    for (int i = 0; i < rows; i++)//Matrisin turune gore icine doldurdugum bolum
    {
        for (int k = 0; k < columns; k++)
        {
            if(value == -10000){//birim matris oluşturma
                if (rows!=columns)
                {
                    cout << "error : could be unit matrix logic error(example 5,5,'e')"<<endl;
                    exit(1);
                }
                else
                {
                    if(i == k){
                        matris[i][k] = 1;
                    }
                    else
                    {
                        matris[i][k] = 0;
                    }
                }

            }
            else if(value==-20000){//random matris oluşturma
                matris[i][k] = rand() % 256;
            }
            else{//sabit sayı matris oluşturma
                matris[i][k] = value;
            }
        }
    }
}

//------------------------------Matris Silme--------------------------------------
template<typename V>
void Matrix<V>::matrisDelete(){
    for (int i = 0; i < this->rows; i++)//Matris değişkenini temizliyoruz.
    {
        delete [] *(this->matris+i);
    }

    delete [] this->matris;
}

//------------------------------Default Constructer------------------------
template<typename V>
Matrix<V>::Matrix(){
    this->value = 0;// 10 x 10 matris olusturuluyor.
    matrisCreate(10,10,this->value);
}

//----------------------------Sabit değer üreten Constructor--------------
template<typename V>
Matrix<V>::Matrix(int row, int column, V value){
    this->value = value;// row x column kadar istenilen degerde matris olusturuluyor.
    matrisCreate(row,column,this->value);
}

//-----------------------Birim MATRİS VE Random Matris-------------------
template<typename V>
Matrix<V>::Matrix(int row, int column, char value){
    if (value == 'e' && row==column){
        this->value = -10000;//birim matris
        matrisCreate(row,column,this->value);}
    else if(value == 'r'){
        this->value = -20000;//random matris
        matrisCreate(row,column,this->value);}
    else{
        cout<< "error : fault definition Matrix"<<endl
        << "can be incorrect value entry(last value 'e' and 'r')"<<endl
        << "could be unit matrix logic error(example 5,5,'e')"<<endl;
        exit(1);
    }
}

//---------------------------Destructor------------------------------------
template<typename V>
Matrix<V>::~Matrix(){
    matrisDelete();
}

//------------------------------Print---------------------------------------
template<typename V>
void Matrix<V>::print()const{
    cout << "----------------------------------------------------------"<< endl;
    cout << "    " << rows << " X " << columns << " Matris"<< endl;
    cout << "----------------------------------------------------------"<< endl;
    for (int i = 0; i < rows; i++){//ekrana yazdırma metodu
        for (int k = 0; k < columns; k++){
            cout << setw(5) << matris[i][k] << " ";
        }
        cout << endl<<endl;
    }
}

//-------------------------File Print---------------------------------------
template<typename V>
void Matrix<V>::print(string filename)const{
    ofstream file(filename,ios::app);
    if(file.is_open()){
        file << "----------------------------------------------------------\n";
        file << "    " << rows << " X " << columns << " Matris\n";
        file << "----------------------------------------------------------\n";
        for (int i = 0; i < rows; i++){//dosyaya yazdırma
            for (int k = 0; k < columns; k++){
                file << setw(5) << matris[i][k] << " ";
            }
            file << endl<<endl;
        }
    }
}

//---------------------------Resize-------------------------------------
template<typename V>
void Matrix<V>::resize(int rows,int columns){
    Matrix<int> *temp = new Matrix<int>(rows,columns,this->value);
    if(rows<this->rows){//matrisi yeniden boyutlandırma metodu
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++){
                temp->matris[i][j]=this->matris[i][j];
            }
        }
    }
    else{
        for (int i = 0; i < this->rows; i++){
            for (int j = 0; j < this->columns; j++){
                temp->matris[i][j]=this->matris[i][j];
            }
        }
    }
    matrisDelete();
    this->matris = temp->matris;
    this->rows = temp->rows;
    this->columns = temp->columns;
    this->value = temp->value;
}

//--------------------------(operator+) Matrix----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator+(const Matrix<V> &m){
    if(m.rows == this->rows && m.columns == this->columns){
        Matrix<V> *temp = new Matrix<V>(this->rows,this->columns,m.matris[0][0] + this->matris[0][0]);
        for (int i = 0; i < temp->rows; i++){// m1 = m1 + m2 şeklinde iki matrisi topluyor.
            for (int k = 0; k < temp->columns; k++){
                temp->matris[i][k] = m.matris[i][k] + this->matris[i][k];
            }
        }
        return *temp;
    }
    else{
        cout << "error : wrong matrix collection" << endl;
        exit(1);// matrisler eşit boyutta olmazsa hata basar
    }
}

//--------------------------(operator+) Scaler----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator+(V number){
    Matrix<V> *temp = new Matrix<V>(this->rows,this->columns, number + this->matris[0][0]);
    for (int i = 0; i < temp->rows; i++){// m1 = m1 +5 şeklinde matris ve scaler deger topluyor.
        for (int k = 0; k < temp->columns; k++){
            temp->matris[i][k] = number + this->matris[i][k];
        }
    }
    return *temp;
}

//--------------------------(operator-) Matrix----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator-(const Matrix<V> &m){
    if(m.rows == this->rows && m.columns == this->columns){
        Matrix<V> *temp = new Matrix<V>(this->rows,this->columns,this->matris[0][0] - m.matris[0][0]);
        for (int i = 0; i < temp->rows; i++){// m1 = m1 - m2 şeklinde iki matrisi cikariyor.
            for (int k = 0; k < temp->columns; k++){
                temp->matris[i][k] = this->matris[i][k] - m.matris[i][k];
            }
        }
        return *temp;
    }
    else{
        cout << "error : wrong matrix collection" << endl;
        exit(1);// matrisler eşit boyutta olmazsa hata basar
    }
}

//--------------------------(operator-) Scaler----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator-(V number){
    Matrix<V> *temp = new Matrix<V>(this->rows,this->columns, this->matris[0][0] - number);
    for (int i = 0; i < temp->rows; i++){// m1 = m1 - 5 seklinde matristen scaler deger cikarir.
        for (int k = 0; k < temp->columns; k++){
            temp->matris[i][k] = this->matris[i][k] - number;
        }
    }
    return *temp;
}

//--------------------------(operator*) Matrix----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator*(const Matrix<V> &m){
    if(this->columns == m.rows){
        Matrix<V> *temp = new Matrix<V>(this->rows,m.columns,0);
        for (int i = 0; i < this->rows; i++){// m1 = m1 * m2 matris caprmasi yapar.
            for (int j = 0; j < m.columns; j++){
                for (int k = 0; k < this->columns; k++){
                    temp->matris[i][j] += (this->matris[i][k]*m.matris[k][j]);
                }
            }
        }
        return *temp;
    }
    else{
        cout << "error : wrong matrix not equal first matris column and second matris row" << endl;
        exit(1);//iki matrisin sütun ve satiri birbirine esit degilse hata basar.
    }
}

//--------------------------(operator*) Scaler----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator*(V number){
    Matrix<V> *temp = new Matrix<V>(this->rows,this->columns, this->matris[0][0] * number);
    for (int i = 0; i < temp->rows; i++){// m1 = m1 * 5 islemini yapar.
        for (int k = 0; k < temp->columns; k++){
            temp->matris[i][k] = this->matris[i][k] * number;
        }
    }
    return *temp;
}

//--------------------------(operator/) Scaler----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator/(V number){
    Matrix<V> *temp = new Matrix<V>(this->rows,this->columns, this->matris[0][0] / number);
    for (int i = 0; i < temp->rows; i++){// m1 = m1 / 5 islemini yapar.
        for (int k = 0; k < temp->columns; k++){
            temp->matris[i][k] = this->matris[i][k] / number;
        }
    }
    return *temp;
}

//--------------------------(operator%) Scaler----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator%(V number){
    Matrix<V> *temp = new Matrix<V>(this->rows,this->columns, this->matris[0][0] % number);
    for (int i = 0; i < temp->rows; i++){ // mod alma islemini yapar.
        for (int k = 0; k < temp->columns; k++){
            temp->matris[i][k] = this->matris[i][k] % number;
        }
    }
    return *temp;
}

//--------------------------(operator^) Scaler----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::operator^(V number){
    Matrix<V> *temp = new Matrix<V>(this->rows,this->columns, V(pow(this->matris[0][0],number)));
    for (int i = 0; i < temp->rows; i++){ // verilen sayı ile us alır.
        for (int k = 0; k < temp->columns; k++){
            temp->matris[i][k] = pow(this->matris[i][k],number);
        }
    }
    return *temp;
}

//---------------------------Transpoze------------------------------------------
template<typename V>
Matrix<V>& Matrix<V>::T(){
    Matrix<V> *temp = new Matrix<V>(this->columns,this->rows,this->matris[0][0]);
    for (int i = 0; i < this->rows; i++){//matrisin transpozunu verir.
        for (int j = 0; j < this->columns; j++){
            temp->matris[j][i] = this->matris[i][j];
        }
    }
    return *temp;
}

//-----------------------------Eleman duzeyinde carpma----------------------------
template<typename V>
Matrix<V>& Matrix<V>::emul(const Matrix<V> &m){
    if(m.rows == this->rows && m.columns == this->columns){
        Matrix<V> *temp = new Matrix<V>(this->rows,this->columns,m.matris[0][0] * this->matris[0][0]);
        for (int i = 0; i < temp->rows; i++){// indis indis carpma islemi yapar.
            for (int k = 0; k < temp->columns; k++){
                temp->matris[i][k] = m.matris[i][k] * this->matris[i][k];
            }
        }
        return *temp;
    }
    else{
        cout << "error : wrong matrix collection" << endl;
        exit(1);// iki matrisin satir ve sutunlari esit degile hata basar.
    }
}

//--------------------------------Matrisin Tersi-----------------------------------
template<typename V>
Matrix<V>& Matrix<V>::inv(){
    if (this->rows==this->columns && this->det() != 0)
    {
        Matrix<V> *unit = new Matrix<V>(this->rows,this->columns,'e');
        Matrix<V> *temp = this;// birim matrise cevirerek matrisin tersini buluyoruz.
        float d,k;
        for (int i = 0; i < temp->rows; i++){
            d = float(temp->matris[i][i]);//kosegenleri 1'e ceviriyoruz.
            for (int j = 0; j < temp->columns; j++){
                temp->matris[i][j] = temp->matris[i][j]/d;
                unit->matris[i][j] = unit->matris[i][j]/d;
            }
            for (int x = 0; x < temp->rows; x++){
                if (x!=i){// kosegen olmayan yerleri boldugumuz sayi ile carpıp cıkarıyoruz.
                    k = float(temp->matris[x][i]);
                    for (int j = 0; j < temp->columns; j++){
                        temp->matris[x][j] = temp->matris[x][j] - (temp->matris[i][j]*k);
                        unit->matris[x][j] = float(unit->matris[x][j]) - float(unit->matris[i][j]*k);
                    }
                }
            }
        }
        return *unit;
    }
    else{
        cout<<"error : matrix not equal row and columns or determinant is not of matrix"<<endl;
        exit(1);
    }
}

//----------------------------------Determinant-------------------------------------------
template<typename V>
double Matrix<V>::det(){
    if (this->rows==this->columns)
    {
        double result = 0;
        if (this->rows == 1 && this->columns == 1){
            result = this->matris[0][0];
            return result;// tek boyutlu bir matris ise kenidini dondurecek
        }
        if (this->rows == 2 && this->columns == 2)
        {
            result = this->matris[0][0] * this->matris[1][1] - this->matris[0][1] * this->matris[1][0];
            return result;// iki boyutlu bir matris ise yukarıdaki islemi dondurecek
        }
        for (int i = 0; i < this->rows; i++)
        {
            Matrix<V> *temp = new Matrix<V>(this->rows - 1,this->columns -1,(V)0);
            for (int j = 1; j < this->rows; j++)
            {
                for (int k = 0; k < this->rows; k++)
                {
                    if (k < i)
                    {
                        temp->matris[j - 1][k] = this->matris[j][k];
                    }else if(k > i){
                        temp->matris[j - 1][k - 1] = this->matris[j][k];
                    }
                }
            }
            result += this->matris[0][i] * pow(-1,(int)i) * temp->det();
        }
        return result;// matrisi bole bole ikili matris haline getiriyor ve sonra recursive olarak geriye donduruyor.
    }
    else{
        cout<<"error : matrix not equal row and columns"<<endl;
        exit(1);
    }
}

//-----------------------------------Image Class---------------------------------------
struct rgb
{
    int **red,**green,**blue;
};

template <class P>
class Image:public Matrix<int>{
    private:
        P color;
        int *bmpHeader = new int();// bmp dosya tipinde bilgi indislerini tutuyor.
        string filename, format; // dosyanın ismini ve formatını tutuyor.
        bool isbinary = true;// image binary mi kontrolu icin
        bool iscolor; // image renkli mi?
        bool isimagevalue = false; // dosyadan resim okundu mu?
        int startindis; // bmp dosya tipinde pixeller kacinci satirda basliyor onu tutuyor.
        bool iscolorImage(string filename, string format);//f otograf renkli mi onu kontrol ediyor.
        int bmpInfo(string filename);// pixel degerlerini okuyacagı indisi döndürüyor.
        bool isiconImagebmp(string filename);// bmp için image icon mu?
        void updatePixel(string filename, string format, int indis, int padding);
    public:
        Image();//(255x255 piksel 0 degerli image) varsayılan constructer ✔
        Image(int width,int height);//(width x height 0 degerli image) contructer
        Image(string filename,string format);//Dosyadan piksel ve boyut okuma (.bin || .bmp) constucter ✔
        virtual ~Image();//Destructor

        void imread(string filename,string format);
        void imwrite(string filename,string format);
        void color2gray();
        void gray2binary(int thr);
        void erosion();
        void dilation();
        void opening();
        void closing();
};

//----------------------------------------IscolorImage-------------------------------------
template<class P>
bool Image<P>::iscolorImage(string filename, string format){
    int gen,yuk;
    ifstream fileRead(filename + "." + format, ios::in | ios::binary);
    if(fileRead.is_open()){
        if(format == "bin"){
            yuk = (int)fileRead.get();
            gen = (int)fileRead.get();
        }
        else if(format == "bmp"){
            int start = bmpInfo(filename);
            yuk = bmpHeader[22];
            gen = bmpHeader[18];
            fileRead.seekg(start,ios::beg);
        }
        int i=0,a=0;
        while (!fileRead.eof())
        {
            a++;
            i = (int)fileRead.get();
        }
        if(gen*yuk == a-1){ return false; }
        else{ return true; }
    }
    fileRead.close();
}

//--------------------------------------bmpInfo--------------------------------------------
template<class P>
int Image<P>::bmpInfo(string filename){
    ifstream bmp(filename + ".bmp", ios::in | ios::binary);
    bmp.seekg(10,ios::beg);// bmp dosyalarındaki bilgi indisini ceker ve bmpHeader'da saklar.
    int startindis = (!isiconImagebmp(filename)) ? (int)bmp.get() : (int)bmp.get() + 1024 ;
    bmpHeader = new int[startindis];
    bmp.seekg(0,ios::beg);
    for (int i = 0; i < startindis; i++){
        bmpHeader[i]=(int)bmp.get();
    }
    bmp.close();
    return startindis;

}

//--------------------------------------IsiconImagebmp------------------------------------
template<class P>
bool Image<P>::isiconImagebmp(string filename){//BMP dosyalarındaki özel durum (icon olup olmadığını kontrol ediyor.)
    ifstream fileRead(filename + ".bmp", ios::in | ios::binary);
    fileRead.seekg(18,ios::beg);// 18. indis genisligi verir.
    int gen = (int)fileRead.get();
    fileRead.seekg(22,ios::beg);// 22. indis yuksekligi verir.
    int yuk = (int)fileRead.get();
    fileRead.seekg(10,ios::beg);// 10. indis pixellerin kacinci indisten basladıgını gosterir.
    int startindis = (int)fileRead.get();
    int padding = 4 - (3 * gen)%4;
    if (padding == 4) { padding = 0; }
    int a=0;
    fileRead.seekg(startindis,ios::beg);
    while(!fileRead.eof()){
        a++;
        fileRead.get();
        if(a%gen==0){ fileRead.seekg(padding,ios::cur); }
    }
    fileRead.close();
    if((a-1) - gen*yuk == 1024 || (a-1) - gen*yuk*3 == 1024) { return true; }
    else{ return false; }
}

//--------------------------------------Destructor---------------------------------------
template<class P>
Image<P>::~Image(){
    delete bmpHeader;
}

//------------------------------------Default Constructer--------------------------------
template <class P>
Image<P>::Image(){
    this->rows=255;// 255 x 255 lik image sınıfı olusturur.
    this->columns=255;
    matrisCreate(this->rows,this->columns,0);
    color.red = matris;
    matrisCreate(this->rows,this->columns,0);
    color.green = matris;
    matrisCreate(this->rows,this->columns,0);
    color.blue = matris;
}

//-----------------------------------(width x height) Constructer------------------------
template <class P>
Image<P>::Image(int width,int height){
    this->columns = width; // width x height kadar image sınıfı olusturur.
    this->rows = height;
    matrisCreate(this->rows,this->columns,0);
    color.red = matris;
    matrisCreate(this->rows,this->columns,0);
    color.green = matris;
    matrisCreate(this->rows,this->columns,0);
    color.blue = matris;
}

//----------------------------------UpdatePixel-------------------------------------------
template<class P>
void Image<P>::updatePixel(string filename, string format, int indis, int padding){
    ifstream fileRead(filename + "." + format, ios::in | ios::binary);
    fileRead.seekg(indis-1,ios::beg);// dosyalardaki pixel verilerini matrislere isler.
    if(fileRead.is_open()){// resim gri ise
        if (!iscolor){
            while (fileRead.get() != -1)
            {
                for (int i = 0; i < this->rows; i++)
                {
                    for (int j = 0; j < this->columns; j++)
                    {
                        color.green[i][j] = (int)fileRead.get();
                        //dosyanın binary olup olmadığını kontrol ediyor.
                        if(color.green[i][j] != 0 ) { if(color.green[i][j] != 255) { isbinary = false;} }
                    }
                    fileRead.seekg(padding,ios::cur);
                }
            }
        }
        else{//fotograf renkli ise
            isbinary = false;
            while (fileRead.get() != -1)
            {
                for (int i = 0; i < this->rows; i++)
                {
                    for (int j = 0; j < this->columns; j++)
                    {
                        color.red[i][j] = (int)fileRead.get();
                        color.green[i][j] = (int)fileRead.get();
                        color.blue[i][j] = (int)fileRead.get();
                    }
                    fileRead.seekg(padding,ios::cur);
                }
            }
        }
    }
    fileRead.close();
}

//----------------------------------(Dosyadan okuyan) Consturcter------------------------
template <class P>
Image<P>::Image(string filename, string format){
    this->imread(filename,format);
}

//-------------------------------------------Imread--------------------------------------
template<class P>
void Image<P>::imread(string filename,string format){
    if (format == "bin"){
        isimagevalue = true;
        this->iscolor = iscolorImage(filename,format);
        ifstream fileRead(filename + "." + format, ios::in | ios::binary);
        if(fileRead.is_open()){
            this->filename = filename;
            this->format = format;
            this->rows = (int)fileRead.get();// bin dosyasındaki ilk iki indis okunuyor.
            this->columns = (int)fileRead.get();
            matrisCreate(this->rows,this->columns,0);
            color.red = matris;// matrisler olusturuluyor.
            matrisCreate(this->rows,this->columns,0);
            color.green = matris;
            matrisCreate(this->rows,this->columns,0);
            color.blue = matris;

            updatePixel(filename,format,2,0);// dosyadaki piksel degerleri matrislere kaydedilmesi icin updatepixel cagırılıyor.
            fileRead.close();
        }
        else{ cout<<"error : no such file"<<endl; exit(1); }
    }
    else if(format == "bmp"){
        isimagevalue = true;
        this->iscolor = iscolorImage(filename,format);
        ifstream fileRead(filename + "." + format, ios::in | ios::binary);
        if(fileRead.is_open()){
            this->filename = filename;
            this->format = format;
            this->startindis = bmpInfo(filename);
            this->columns = bmpHeader[18];
            this->rows = bmpHeader[22]; // bmpHeader'dan gerekli bilgiler alınıyor.
            matrisCreate(this->rows,this->columns,0);
            color.red = matris;
            matrisCreate(this->rows,this->columns,0);
            color.green = matris;
            matrisCreate(this->rows,this->columns,0);
            color.blue = matris;

            int padding = 4 - (3 * this->rows)%4;
            if (padding == 4) { padding = 0; }// bmp dosyalarındaki padding kontrolu yapılıyor.

            updatePixel(filename,format,startindis,padding);
            fileRead.close();
        }
        else{ cout<<"error : no such file"<<endl; exit(1); }
    }

    else{// bin veya bmp haricinde bir dosya girilirse hata verir.
        cout<<"error : wrong file format(bin or bmp)"<<endl;     exit(1);
    }
}

//-------------------------------------------Imrwrite--------------------------------------
template<class P>
void Image<P>::imwrite(string filename,string format){
    if ((format == "bin" || format == "bmp") && isimagevalue)
    {
        ofstream fileWrite(filename + "." + format, ios::out | ios::binary);
        if (format == "bin"){ // yukseklik ve genislik degerleri dosyaya yazdırılıyor.
            fileWrite.write((char*)&this->rows,sizeof(char));
            fileWrite.write((char*)&this->columns,sizeof(char));
        }
        else{//bmp dosya formatında bilgi indisleri dosyaya yazdırılıyor.
            for (int i = 0; i < this->startindis; i++)
            {
                fileWrite.write((char*)&bmpHeader[i],sizeof(char));
            }
        }

        if(!iscolor){// renkli degil ise green matrisi dosyaya yazdırılıyor.
            for (int i = 0; i < this->rows; i++)
            {
                for (int j = 0; j < this->columns; j++)
                {
                    fileWrite.write((char*)&color.green[i][j],sizeof(char));
                }
            }
        }
        else{// renkli ise tum matrisler dosyaya yazdırılıyor.
            for (int i = 0; i < this->rows; i++)
            {
                for (int j = 0; j < this->columns; j++)
                {
                    fileWrite.write((char*)&color.red[i][j],sizeof(char));
                    fileWrite.write((char*)&color.green[i][j],sizeof(char));
                    fileWrite.write((char*)&color.blue[i][j],sizeof(char));
                }
            }
        }
        fileWrite.close();
    }
    else{
        cout<<"error : wrong file format(bin or bmp) or image matrix empty"<<endl;
        exit(1);
    }
}

//-------------------------------------------Color2Gray------------------------------------
template<class P>
void Image<P>::color2gray(){
    if (iscolor && !isbinary && isimagevalue){
        iscolor = false;
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->columns; j++)// renkli dosyayı gri seviyeye ceviriyoruz.
            {
               color.green[i][j] = 0.299*color.red[i][j] + 0.587*color.green[i][j] + 0.114*color.blue[i][j];
            }
        }
    }
    else{// resim gri ise hicbir islem yapilmayacak.
        cout<<"caution : image is gray or image matrix empty"<<endl; exit(1);
    }
}

//-------------------------------------------Gray2Binary-----------------------------------
template<class P>
void Image<P>::gray2binary(int thr){
    if (!iscolor  && isimagevalue){
        isbinary = true; // gri resmi binary resme donusturuyor.
        for (int i = 0; i < this->rows; i++){
            for (int j = 0; j < this->columns; j++){
                if(color.green[i][j] < thr)
                    color.green[i][j] = 0;
                else
                    color.green[i][j] = 255;
            }
        }
    }
    else{// resim renkli veya matris bos ise hata verir.
        cout<<"caution : image is not gray or image matrix empty"<<endl; exit(1);
    }
}

//-------------------------------------------Erosion---------------------------------------
template<class P>
void Image<P>::erosion(){
    if(isbinary){
        int count = 0;//degisen indisleri tutmak icin iki dizi olusturuldu.
        int rowsindis[this->rows * this->columns];
        int columnsindis[this->rows * this->columns];
        for (int i = 0; i < this->rows; i++){
            for (int j = 0; j < this->columns; j++)
            {
                bool ischeck = false;
                if(color.green[i][j] == 0){
                    for (int m = 0; m < count; m++)
                    {
                        if(rowsindis[m] == i && columnsindis[m] == j){
                            ischeck = true;
                            break;// eger o indis 1'dan 0'e cevrilmis ise atlıyor.
                        }
                    }
                    if(ischeck == true){ continue; }

                    if(i>0){ // [0,1,0], [1,1,1], [0,1,0] seklinde matris dolastırılıyor
                        if(color.green[i-1][j] != 0){// deger 0'a esit ise arti seklinde degerler
                            color.green[i-1][j] = 0;// degististiriliyor.
                        }
                    }

                    if(j>0){
                        if(color.green[i][j-1] != 0){
                            color.green[i][j-1] = 0;
                        }
                    }

                    if(i < this->rows - 1){
                        if(color.green[i+1][j] != 0){
                            color.green[i+1][j] = 0;
                            rowsindis[count] = i+1;
                            columnsindis[count] = j;
                            count++;
                        }
                    }
                    if(j < this->columns - 1){
                        if(color.green[i][j+1] != 0){
                            color.green[i][j+1] = 0;
                            rowsindis[count] = i;
                            columnsindis[count] = j+1;
                            count++;
                        }
                    }
                }
                else{
                    continue;
                }
            }
        }
    }
    else{// dosya binary degil ise hata veriyor.
        cout<<"error : file is not binary"<<endl; exit(1);
    }
}

//-------------------------------------------Dilation---------------------------------------
template<class P>
void Image<P>::dilation(){
    if(isbinary){
        int count = 0;//degisen indisleri tutmak icin iki dizi olusturuldu.
        int rowsindis[this->rows * this->columns];
        int columnsindis[this->rows * this->columns];
        for (int i = 0; i < this->rows; i++){
            for (int j = 0; j < this->columns; j++)
            {
                bool ischeck = false;
                if(color.green[i][j] == 255){
                    for (int m = 0; m < count; m++)
                    {
                        if(rowsindis[m] == i && columnsindis[m] == j){
                            ischeck = true;
                            break;// eger o indis 0'dan 1'e cevrilmis ise atlıyor.
                        }
                    }
                    if(ischeck == true){ continue; }

                    if(i>0){ // [0,1,0], [1,1,1], [0,1,0] seklinde matris dolastırılıyor
                        if(color.green[i-1][j] != 255){ // deger 0'a esit ise arti seklinde degerler
                            color.green[i-1][j] = 255;// degististiriliyor.
                        }
                    }

                    if(j>0){
                        if(color.green[i][j-1] != 255){
                            color.green[i][j-1] = 255;
                        }
                    }

                    if(i < this->rows - 1){
                        if(color.green[i+1][j] != 255){
                            color.green[i+1][j] = 255;
                            rowsindis[count] = i+1;
                            columnsindis[count] = j;
                            count++;
                        }
                    }
                    if(j < this->columns - 1){
                        if(color.green[i][j+1] != 255){
                            color.green[i][j+1] = 255;
                            rowsindis[count] = i;
                            columnsindis[count] = j+1;
                            count++;
                        }
                    }
                }
                else{
                    continue;
                }
            }
        }
    }
    else{// dosya binary degil ise hata veriyor.
        cout<<"error : file is not binary"<<endl; exit(1);
    }
}

//-------------------------------------------Opening---------------------------------------
template<class P>
void Image<P>::opening(){
    erosion();
    dilation();
}

//-------------------------------------------Closing---------------------------------------
template<class P>
void Image<P>::closing(){
    dilation();
    erosion();
}

//----------------------------------------Table Sınıf--------------------------------------
template<class M>
class Table:public Matrix<int>{
    private:
        string *rowt, *columnt;
        void columnsandRowsCreate(int row,int column);
        int search(string *arr,int n, string x);
    public:
        Table();
        Table(int,int,M);
        Table(int,int,char);
        virtual ~Table();

        virtual void print()const;
        M itemAt(int,int);
        M itemAt(string);
        M itemAt(string,string);
        void setRowNames(string s[],int n);
        void setColNames(string s[],int n);
};

//----------------------------------Columns and Rows Create-------------------------------
template<class M>
void Table<M>::columnsandRowsCreate(int row,int column){
    rowt = new string[row];//satır ve sutun isimlerini varsayılan olusturan metod
    columnt = new string[column];
    for (int i = 0; i < row; i++){
        rowt[i]=to_string(i);
    }
    for (int i = 0; i < column; i++){
        columnt[i]=i+65;
    }
}

//--------------------------------------linear search algrothm----------------------------
template<class M>
int Table<M>::search(string *arr,int n, string x) {
    for (int i = 0; i < n; i++)
        if (arr[i] == x)
            return i;
    return -1;
}

//----------------------------------Default Constructor------------------------------------
template<class M>
Table<M>::Table(){
    //matrisCreate(10,10,0);// 10 x 10 tablo olusturuyor.
    columnsandRowsCreate(10,10);
}

//----------------------------------(r x c) Table Constructer------------------------------
template<class M>
Table<M>::Table(int r,int c, M d){
    matrisCreate(r,c,d);// r x c tablo olusturuyor.
    columnsandRowsCreate(r,c);
}

//------------------------------------Random Table Constructor-----------------------------
template<class M>
Table<M>::Table(int r, int c, char ch){
    if(ch == 'r'){ // r x c random degerli tablo olusturuyor.
        matrisCreate(r,c,-20000);
        columnsandRowsCreate(r,c);
    }
    else{ cout<<"error : 'r' input for random value"<<endl; exit(1); }
}

//------------------------------------Destructor------------------------------------------
template<class M>
Table<M>::~Table(){
    delete[] rowt;
    delete[] columnt;
}

//---------------------------------------Print---------------------------------------------
template<class M>
void Table<M>::print()const{
    cout << "----------------------------------------------------------"<< endl;
    cout << "    " << this->rows << " X " << this->columns << " Table"<< endl;
    cout << "----------------------------------------------------------"<< endl<<endl;
    for (int i = -1; i < this->rows; i++)
    {
        if(i != -1) {cout << setw(9) << rowt[i] << " |";} else{ cout<<setw(11) << "###"; }
        for (int k = 0; k < this->columns; k++){
            if(i == -1) { cout << setw(7) << columnt[k] << " ";}
            else{ cout << setw(7) << matris[i][k] << " "; }
        }
        cout << endl<<endl;//ekrana yazdırma fonksiyonu
    }
}

//--------------------------------------itemAt(int,int)-----------------------------------
template<class M>
M Table<M>::itemAt(int r, int c){
    if(this->rows < r || this->columns < c){
      cout<<"error : wrong indis"<<endl; exit(1);
    }
    else{
      return matris[r][c]; // belitilen indisleri donduruyor.
    }
}

//-------------------------------------itemAt(string)-------------------------------------
template<class M>
M Table<M>::itemAt(string s){
    if(s.length() == 3 || s.length() == 2){//2 veya 3'ten buyuk olursa fonksiyon calısmaz.
        string c  =  s.substr(1);
        string d = s.substr(0,1);
        int a = search(rowt,this->rows,c); int b = search(columnt,this->columns,d);
        if (a != -1 && b != -1){
            return matris[a][b];
        }
        else{ cout<<"information : No results found. You may have entered the wrong value"<<endl; exit(1);}
        cout<<c<<endl;
    }
    else{// fonksiyon calismaz ise kullanicinin ne yapmasi gerektigini anlatan cikti
        cout<<"error: Please proceed according to the default table."
        <<"You may have updated the default table."<<endl<<"if you did, use the following function itemAt(string,string)"
        <<endl; exit(1);
    }
}

//------------------------------------itemAt(string,string)------------------------------
template<class M>
M Table<M>::itemAt(string rs, string cs){
    int b = search(columnt,this->columns,cs);// gonderilen string degerler listede varsa
    int a = search(rowt,this->rows,rs);// indis degeri dondurulur.
    if(a != -1 && b != -1){
        return matris[a][b];
    }
    else{ cout<<"error : No results found. You may have entered the wrong value"<<endl; exit(1); }
}

//----------------------------------------Setrows----------------------------------------
template<class M>
void Table<M>::setRowNames(string s[],int n){
    for (int i = 0; i < n; i++){//satır guncellemesi yapilir.
        rowt[i] = s[i];
    }
}

//----------------------------------------Setrows----------------------------------------
template<class M>
void Table<M>::setColNames(string s[],int n){
    for (int i = 0; i < n; i++){//sutun guncellemesi yapilir.
        columnt[i] = s[i];
    }
}

int main(){

    return 0;
}
