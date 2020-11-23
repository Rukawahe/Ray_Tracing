#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <exception>

#include <vector>
//图像输出 过滤
class image
{
public:

    struct rgb
    {
        rgb():r(0),g(0),b(0){}
        rgb(float rr):r(rr),g(rr),b(rr){}
        rgb(float r,float g,float b):r(r),g(g),b(b){}
        float r,g,b;

        bool operator !=(const rgb & c)const{
            return c.r!=r && c.g!=g && c.b!=b;
        }
        rgb& operator *=(const rgb &c){
            r*=c.r,g*=c.g,b*=c.b;
            return *this;
        }
        rgb& operator +=(const rgb &c){
            r+=c.r,g+=c.g,b+=c.b;
            return *this;
        }
        friend float& operator+=(float &f ,const rgb c){
            f+=(c.r+c.g+c.b)/3.f;
            return f;
        }
    };

    image():width(0),height(0),pixel(nullptr){}
    image(const unsigned int &_w,const unsigned int &_h,const rgb &c=kBlack):
                width(_w),height(_h),pixel(nullptr){
        pixel=new rgb[width*height];
        for(int i=0;i<width*height;i++)    pixel[i]=c;
            
    }
    image(const image &img):width(img.width),height(img.height),pixel(nullptr){
        pixel=new rgb[width*height];
        memcpy(pixel,img.pixel,sizeof(rgb)*width*height);
    }

    //移动构造函数
    image(image &&img):width(0),height(0),pixel(nullptr){
        width=img.width;
        height=img.height;
        pixel=img.pixel;
        img.pixel=nullptr;
        width=0,height=0;
    }
    image& operator = (image &&img){
        if(this!=&img){
            if(pixel!=nullptr)  delete pixel;
            width=img.width;
            height=img.height;
            pixel=img.pixel;
            img.pixel=nullptr;
            width=0,height=0;
        }
        return *this;
    }
    rgb& operator ()(const unsigned int &x,const unsigned int &y)const{
        assert(x<width&& y<height);//如果条件返回错误，则终止程序执行
        return pixel[y*width+x];
    }
    image& operator *= (const rgb &rgb)
    {
        for (int i = 0; i < width * height; ++i) pixel[i] *= rgb;
        return *this;
    }
    image& operator += (const image &img)
    {
        for (int i = 0; i < width * height; ++i) pixel[i] += img[i];
        return *this;
    }
    image& operator /= (const float &div)
    {
        float invDiv = 1 / div;
        for (int i = 0; i < width * height; ++i) pixel[i] *= invDiv;
        return *this;
    }
    friend image operator * (const rgb &rgb, const image &img)
    {
        image tmp(img); 
        tmp *= rgb;
        return tmp;
    }
    image operator * (const image &img)
    {
        image tmp(*this); 
        // multiply pixels together
        for (int i = 0; i < width * height; ++i) tmp[i] *= img[i];
        return tmp;
    }
    static image circshift(const image &img, const std::pair<int,int> &shift)
    {
        image tmp(img.width, img.height);
        int w = img.width, h = img.height;
        for (int j = 0; j < h; ++j) {
            int jmod = (j + shift.second) % h;
            for (int i = 0; i < w; ++i) {
                int imod = (i + shift.first) % w;
                tmp[jmod * w + imod] = img[j * w + i];
            }    
        }
        return tmp;
    }

    const rgb& operator [](const unsigned int &i)const{
        return pixel[i];
    }
    rgb& operator[](const unsigned int &i){
        return pixel[i];
    }
    ~image(){   if(pixel!=nullptr) delete []pixel;}
    unsigned int width,height;
    rgb *pixel;
    static const rgb kBlack,kWhite,kRed,kGreen,kBlue;

};
//颜色设置
const image::rgb image::kBlack = image::rgb(0);
const image::rgb image::kWhite = image::rgb(1);
const image::rgb image::kRed = image::rgb(1,0,0);
const image::rgb image::kGreen = image::rgb(0,1,0);
const image::rgb image::kBlue = image::rgb(0,0,1);

//保存ppm文件
void saveppm(const image &img, const char *filename)
{
    if (img.width == 0 || img.height == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // 以二进制打开
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << img.width << " " << img.height << "\n255\n";
        unsigned char r, g, b;
        for (int i = 0; i < img.width * img.height; ++i) {
            r = static_cast<unsigned char>(std::min(1.f, img.pixel[i].r) * 255);
            g = static_cast<unsigned char>(std::min(1.f, img.pixel[i].g) * 255);
            b = static_cast<unsigned char>(std::min(1.f, img.pixel[i].b) * 255);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}
//读取ppm文件     注意返回值优化，返回指针类型
image readPPM(const char *filename) 
{ 
    std::ifstream ifs; 
    ifs.open(filename, std::ios::binary); 
    image img; 
    try { 
        if (ifs.fail()) { throw("Can't open input file"); } 
        std::string header; 
        int w, h, b; 
        ifs>>header; 
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file"); 
        ifs >> w >> h >> b; 
        img.width = w; img.height = h; 
        img.pixel = new image::rgb[w * h]; 
        ifs.ignore(256, '\n'); // 跳过空行
        unsigned char pix[3]; 
        
        for (int i = 0; i < w * h; ++i) { 
            ifs.read(reinterpret_cast<char *>(pix), 3); 
            img.pixel[i].r = pix[0] / 255.f; 
            img.pixel[i].g = pix[1] / 255.f; 
            img.pixel[i].b = pix[2] / 255.f; 

            //散景效果
            if (img.pixel[i].r > 0.7) img.pixel[i].r *= 3;
            if (img.pixel[i].g > 0.7) img.pixel[i].g *= 3;
            if (img.pixel[i].b > 0.7) img.pixel[i].b *= 3;
        } 
        ifs.close(); 
    } 
    catch (const char *err) { 
        fprintf(stderr, "%s\n", err); 
        ifs.close(); 
    } 
 
    return img; 
}
int main(int argc,char **argv){
    try {
        image I = readPPM("xmas.ppm");
        image J = readPPM("heart.ppm");
        int w = J.width, h = J.height;
        image K(w, h);
        float total = 0;
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                if (J(i, j) != image::kBlack) {
                    K += J(i, j) * image::circshift(I, std::pair<int, int>(i, j));
                    total += J(i, j);
                }
            }
        }
        K /= total;
        saveppm(K, "output.ppm");
    }
    catch (const std::exception &e) { // catch general exception (bad_alloc mainly?)
        fprintf(stderr, "Error: %s\n", e.what());
    }

    return 0;
}


