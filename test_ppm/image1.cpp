#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cassert>
#include <exception>
class image
{
public:

    struct rgb
    {
        rgb():r(0),g(0),b(0){}
        rgb(float rr):r(rr),g(rr),b(rr){}
        rgb(float r,float g,float b):r(r),g(g),b(b){}
        float r,g,b;

        /*bool operator !=(const rgb & c)const{
            return c.r!=r || c.g!=g || c.b!=b;
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
        }*/
    };

    image():width(0),height(0),pixel(nullptr){}
    image(const unsigned int &_w,const unsigned int &_h):
                width(_w),height(_h),pixel(nullptr){
        pixel=new rgb[width*height];
        for(int i=0;i<width*height;i++)    pixel[i]=0;
            
    }
    /*const rgb& operator [](const unsigned int &i)const{
        return pixel[i];
    }
    rgb& operator[](const unsigned int &i){
        return pixel[i];
    }*/
    ~image(){   if(pixel!=nullptr) delete []pixel;}
    unsigned int width,height;
    rgb *pixel;

};
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
//读取ppm文件
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
    image i=readPPM("xmas.ppm");
    saveppm(i,"out3.ppm");
    return 0;
}


