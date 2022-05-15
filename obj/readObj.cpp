#include <GLFW/glfw3.h>
#include <GLUT/glut.h>  
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdlib.h>  
#include "readObj.h"
using namespace std;

//bunny使用1000， cube和eight使用2
#define MOD_VALUE 2//800//2//1000
//cube bunny Eight
const char* objName = "objFile/Eight.obj";
double weightX = 0;
double weightY = 0;
double weightZ = 0;


void readPicture(const char* name);
void drawTriangle();
void drawLine();
void offset();

//读取obj文件并显示三角网格
int main(int argc, char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    readPicture(objName);
    offset();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        drawTriangle();
        drawLine();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();   
    }

    glfwTerminate();
    return 0;
}

/*read obj file */
void readPicture(const char* name) 
{
    ifstream ifs(name);//cube bunny Eight
    string s;
    Facet *f;
    POINT3 *v;
    NormalVector *vn;
    Texture *vt;
    while(getline(ifs,s))
    {
        if(s.length()<2)continue;
        if(s[0]=='v'){
            if(s[1]=='t'){//vt 0.581151 0.979929 纹理
                istringstream in(s);
                vt=new Texture();
                string head;
                in>>head>>vt->TU>>vt->TV;
                m_pic.VT.push_back(*vt);
            }else if(s[1]=='n'){//vn 0.637005 -0.0421857 0.769705 法向量
                istringstream in(s);
                vn=new NormalVector();
                string head;
                in>>head>>vn->NX>>vn->NY>>vn->NZ;
                m_pic.VN.push_back(*vn);
            }else{//v -53.0413 158.84 -135.806 点
                istringstream in(s);
                v=new POINT3();
                string head;
                in>>head>>v->X>>v->Y>>v->Z;
                weightX += v->X;
                weightY += v->Y;
                weightZ += v->Z;

                m_pic.V.push_back(*v);
            }
        }
        else if(s[0]=='f'){//f 2443//2656 2442//2656 2444//2656 面
            for(int k=s.size()-1;k>=0;k--){
                if(s[k]=='/')s[k]=' ';
            }
            istringstream in(s);
            f=new Facet();
            string head;
            in>>head;
            int i=0;
            while(i<3)
            {
                if(m_pic.V.size()!=0)
                {
                    in>>f->V[i];
                    f->V[i]-=1;
                }
                if(m_pic.VT.size()!=0)
                {
                    in>>f->T[i];
                    f->T[i]-=1;
                }
                if(m_pic.VN.size()!=0)
                {
                    in>>f->N[i];
                    f->N[i]-=1;
                }
                i++;
            }
            m_pic.F.push_back(*f);
        }
    }
}

/*偏移*/
void offset() 
{
    weightX /= m_pic.V.size();
    weightY /= m_pic.V.size();
    weightZ /= m_pic.V.size();

    for (int i=0; i< m_pic.V.size(); i++) {
        m_pic.V[i].X -= weightX;
        m_pic.V[i].Y -= weightY;
        m_pic.V[i].Z -= weightZ;
    }
}

/*render*/
void drawTriangle() 
{

    for(int i=0;i<m_pic.F.size();i++)
    {
        glBegin(GL_TRIANGLES);      

        glColor3f(0, 1.0, 0.0);

        //绘制三角形
        if(m_pic.VT.size()!=0)
            glTexCoord2f(m_pic.VT[m_pic.F[i].T[0]].TU,m_pic.VT[m_pic.F[i].T[0]].TV);  //纹理  
        if(m_pic.VN.size()!=0)
            glNormal3f(m_pic.VN[m_pic.F[i].N[0]].NX,m_pic.VN[m_pic.F[i].N[0]].NY,m_pic.VN[m_pic.F[i].N[0]].NZ);//法向量
        glVertex3f(m_pic.V[m_pic.F[i].V[0]].X/MOD_VALUE,m_pic.V[m_pic.F[i].V[0]].Y/MOD_VALUE, m_pic.V[m_pic.F[i].V[0]].Z/MOD_VALUE);        // 上顶点

        if(m_pic.VT.size()!=0)
            glTexCoord2f(m_pic.VT[m_pic.F[i].T[1]].TU,m_pic.VT[m_pic.F[i].T[1]].TV);  //纹理
        if(m_pic.VN.size()!=0)
            glNormal3f(m_pic.VN[m_pic.F[i].N[1]].NX,m_pic.VN[m_pic.F[i].N[1]].NY,m_pic.VN[m_pic.F[i].N[1]].NZ);//法向量
        glVertex3f(m_pic.V[m_pic.F[i].V[1]].X/MOD_VALUE,m_pic.V[m_pic.F[i].V[1]].Y/MOD_VALUE, m_pic.V[m_pic.F[i].V[1]].Z/MOD_VALUE);        // 左下

        if(m_pic.VT.size()!=0)
            glTexCoord2f(m_pic.VT[m_pic.F[i].T[2]].TU,m_pic.VT[m_pic.F[i].T[2]].TV);  //纹理
        if(m_pic.VN.size()!=0)
            glNormal3f(m_pic.VN[m_pic.F[i].N[2]].NX,m_pic.VN[m_pic.F[i].N[2]].NY,m_pic.VN[m_pic.F[i].N[2]].NZ);//法向量
        glVertex3f(m_pic.V[m_pic.F[i].V[2]].X/MOD_VALUE,m_pic.V[m_pic.F[i].V[2]].Y/MOD_VALUE, m_pic.V[m_pic.F[i].V[2]].Z/MOD_VALUE);        // 右下
        glEnd();            
        // 三角形绘制结束                          
    }   
}

void drawLine() 
{
    for(int i=0;i<m_pic.F.size();i++)
    {
        glBegin(GL_LINE_LOOP);

        glColor3f(1.0, 0.0, 0.0);

        GLfloat pt1[3] = {m_pic.V[m_pic.F[i].V[0]].X/MOD_VALUE, m_pic.V[m_pic.F[i].V[0]].Y/MOD_VALUE, m_pic.V[m_pic.F[i].V[0]].Z/MOD_VALUE};
        GLfloat pt2[3] = {m_pic.V[m_pic.F[i].V[1]].X/MOD_VALUE, m_pic.V[m_pic.F[i].V[1]].Y/MOD_VALUE, m_pic.V[m_pic.F[i].V[1]].Z/MOD_VALUE};
        GLfloat pt3[3] = {m_pic.V[m_pic.F[i].V[2]].X/MOD_VALUE, m_pic.V[m_pic.F[i].V[2]].Y/MOD_VALUE, m_pic.V[m_pic.F[i].V[2]].Z/MOD_VALUE};
        glVertex2fv(pt1);
        glVertex2fv(pt2);
        glVertex2fv(pt3);

        glEnd();
    }
}
