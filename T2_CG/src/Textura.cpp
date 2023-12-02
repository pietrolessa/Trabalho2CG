// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa b�sico para criar aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
    static DWORD last_idle_time;
#else
    #include <sys/time.h>
    static struct timeval last_idle_time;
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif


#include "SOIL/SOIL.h"

#include "TextureClass.h"

#include "ListaDeCoresRGB.h"

#include "Ponto.h"

GLfloat AspectRatio, AngY=0;
GLuint TEX1, TEX2;
bool ComTextura = true;

Ponto origem = Ponto (0,0,0);
Ponto posicaoCanhao = Ponto(1,0,11.5);
Ponto direcaoVeiculo = Ponto(1,0,0);

float anguloBaseCanhao = 0.0;
float velocidadeCanhao = 2.5;
float velocidadeRotacao = 4;

Ponto posicaoInicialProjetil = Ponto (0,0,0);
Ponto posicaoProjetil = Ponto(1,0,11.5);
float anguloBraco1 = 0.0;
float anguloBraco2=0.0;


//Define a textura a ser usada
void initTexture (void)
{
    TEX1 = LoadTexture ("grass.jpg");
    TEX2 = LoadTexture ("netherbrick.jpg");
}

// **********************************************************************
//  void DefineLuz(void)

void DefineLuz(void)
{
    // Define cores para um objeto dourado
    GLfloat LuzAmbiente[] = {0.4, 0.4, 0.4};
    GLfloat LuzDifusa[] = {0.7, 0.7, 0.7};
    GLfloat LuzEspecular[] = {0.9, 0.9, 0.9};
    GLfloat PosicaoLuz0[] = {0.0f, 3.0f, 5.0f, 1.0}; // Posicao da Luz
    GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

    // ****************  Fonte de Luz 0

    glEnable(GL_COLOR_MATERIAL);

    // Habilita o uso de iluminação
    glEnable(GL_LIGHTING);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);

    // Define os parâmetros da luz número Zero
    glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0);
    glEnable(GL_LIGHT0);

    // Ativa o "Color Tracking"
    glEnable(GL_COLOR_MATERIAL);

    // Define a reflectância do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, Especularidade);

    // Define a concentração do brilho.
    // Quanto maior o valor do segundo parâmetro, mais
    // concentrado será o brilho. (Valores válidos: de 0 a 128)
    glMateriali(GL_FRONT, GL_SHININESS, 100);
}



// **********************************************************************
//  void init(void)
//		Inicializa os parametros globais de OpenGL
//
// **********************************************************************
void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo de tela preto

	//glShadeModel(GL_SMOOTH);
    glShadeModel(GL_FLAT);
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	glEnable(GL_DEPTH_TEST);
	glEnable ( GL_CULL_FACE );
    //glDisable ( GL_CULL_FACE );

    glEnable(GL_NORMALIZE);
    initTexture();

}
// **********************************************************************
//  void PosicUser()
// **********************************************************************

void PosicUser()
{
    // Set the clipping volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, AspectRatio, 0.1, 1000);  // Ajuste o ângulo de visão (field of view) e o plano de corte conforme necessário

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Posição da câmera (frontal)
    GLfloat eyex = 25.0f; // Posição X no meio do chão
    GLfloat eyey = 10.0f;  // Posição Y aproximadamente no meio da altura da parede
    GLfloat eyez = 55.0f; // Posição Z - uma distância para trás da cena para que tudo caiba na visão

    // Ponto para onde a câmera está olhando
    GLfloat centerx = 25.0f; // Mesma posição X do eyex para olhar diretamente para o meio do chão
    GLfloat centery = 5.0f;  // Mesma posição Y do eyey para olhar diretamente para o meio da parede
    GLfloat centerz = 12.0f;  // Olhar para o início do plano Z

    // Direção "para cima" da cena
    GLfloat upx = 0.0f; // Direção X para cima (usualmente 0)
    GLfloat upy = 5.0f; // Direção Y para cima (1 para olhar para o horizonte)
    GLfloat upz = 0.0f; // Direção Z para cima (0 pois não estamos inclinando a cabeça para os lados)

    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}


// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}
// **********************************************************************
//   void DesenhaCubo (GLuint nro_da_textura)
//
//
// **********************************************************************
void DesenhaCubo ()
{
    glBegin ( GL_QUADS );

    // Front Face
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);

    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back Face
    glNormal3f(0, 0, -1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Top Face
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Bottom Face
    glNormal3f(0, -1, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face
    glNormal3f(1, 0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left Face
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();
}

// **********************************************************************
// void DesenhaLadrilho(int idTextura)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaQuadrado(int idTextura)
{
    glBindTexture(GL_TEXTURE_2D, idTextura);

    float repeticaoTextura = 1.0f; // Este valor determina quantas vezes a textura é repetida em cada quadrado

    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, -0.5f);

        glTexCoord2f(0.0f, repeticaoTextura);
        glVertex3f(-0.5f, 0.0f, 0.5f);

        glTexCoord2f(repeticaoTextura, repeticaoTextura);
        glVertex3f(0.5f, 0.0f, 0.5f);

        glTexCoord2f(repeticaoTextura, 0.0f);
        glVertex3f(0.5f, 0.0f, -0.5f);
    glEnd();
}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************

void DesenhaQuadrado(int corBorda, int corDentro)

{

    defineCor(corBorda); // desenha QUAD preenchido
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    defineCor(corDentro);
    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    //defineCor(corDentro);// desenha QUAD preenchido
    glColor3f(1,1,1);
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    //defineCor(corBorda);
    glColor3f(0,1,0);

    glBegin ( GL_LINE_STRIP );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();


}

// **********************************************************************\
//
// **********************************************************************
void DesenhaPiso()
{
    glPushMatrix();
    glTranslated(origem.x, origem.y-1, origem.z);
    for(int x=0; x<50;x++)
    {
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, TEX1);
        for(int z=0; z<25;z++)
        {
            DesenhaCubo();
            glTranslated(0, 0, 1);
        }

        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
    glTranslatef(0,0,0);
}

void DesenhaParede()
{
    glPushMatrix();


        glTranslated(25.0f, origem.y, origem.z);

        glBindTexture(GL_TEXTURE_2D, TEX2);
        for(int x=0; x<15; x++) // Ajuste estas dimensões conforme necessário
        {
            glPushMatrix();
            for(int z=0; z<25; z++)
            {
                DesenhaCubo(); // Desenha um quadrado com textura
                glTranslated(0, 0, 1); // Move para o próximo quadrado
            }
            glPopMatrix();
            glTranslated(0, 1, 0); // Move para a próxima linha de quadrados
        }
    glPopMatrix();
    // Ajuste as transformações conforme necessário
    glTranslatef(0,0,0);
}


void DesenhaBraco2 ()
{
glPushMatrix();


    float largura = 0.5f;
    float altura = 1.0f;
    float profundidade = 0.5f; // Defina a profundidade conforme necessário

    // Translada para o ponto central da base do paralelepípedo
    glTranslatef(largura / 2, 0, profundidade / 2);
    glRotatef(anguloBraco1, 0, 1, 0); // Rotação em torno do eixo Y
    glTranslatef(-largura / 2, 0, -profundidade / 2);

    //rotamos o braco 2
    glTranslatef(largura/2,0,profundidade/2);
    glRotatef(anguloBraco2,0,0,1);
    glTranslatef(-largura/2,0,-profundidade/2);

    // Define a cor do canhão para cinza
    glColor3f(0.0f,0.0f,1.0f); // RGB para verde

    // Desenha o paralelepípedo (canhão)
    glBegin(GL_QUADS);

    // Topo
    glVertex3f(0, altura, 0);
    glVertex3f(0, altura, profundidade);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(largura, altura, 0);

    // Base
    glVertex3f(0, 0, 0);
    glVertex3f(largura, 0, 0);
    glVertex3f(largura, 0, profundidade);
    glVertex3f(0, 0, profundidade);

    // Frente
    glVertex3f(0, 0, profundidade);
    glVertex3f(largura, 0, profundidade);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(0, altura, profundidade);

    // Traseira
    glVertex3f(0, 0, 0);
    glVertex3f(0, altura, 0);
    glVertex3f(largura, altura, 0);
    glVertex3f(largura, 0, 0);

    // Esquerda
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, profundidade);
    glVertex3f(0, altura, profundidade);
    glVertex3f(0, altura, 0);

    // Direita
    glVertex3f(largura, 0, 0);
    glVertex3f(largura, altura, 0);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(largura, 0, profundidade);

    glEnd();

    glPopMatrix();
}


void DesenhaBraco1()
{

    float largura = 0.5f;
    float altura = 1.0f;
    float profundidade = 0.5f; // Defina a profundidade conforme necessário

    glPushMatrix();
    // Translada para o ponto central da base do paralelepípedo
    glTranslatef(largura / 2, 0, profundidade / 2);
    glRotatef(anguloBraco1, 0, 1, 0); // Rotação em torno do eixo Y
    glTranslatef(-largura / 2, 0, -profundidade / 2);

    // Define a cor do canhão para cinza
    glColor3f(0.0f,1.0f,0.0f); // RGB para verde

    // Desenha o paralelepípedo (canhão)
    glBegin(GL_QUADS);

    // Topo
    glVertex3f(0, altura, 0);
    glVertex3f(0, altura, profundidade);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(largura, altura, 0);

    // Base
    glVertex3f(0, 0, 0);
    glVertex3f(largura, 0, 0);
    glVertex3f(largura, 0, profundidade);
    glVertex3f(0, 0, profundidade);

    // Frente
    glVertex3f(0, 0, profundidade);
    glVertex3f(largura, 0, profundidade);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(0, altura, profundidade);

    // Traseira
    glVertex3f(0, 0, 0);
    glVertex3f(0, altura, 0);
    glVertex3f(largura, altura, 0);
    glVertex3f(largura, 0, 0);

    // Esquerda
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, profundidade);
    glVertex3f(0, altura, profundidade);
    glVertex3f(0, altura, 0);

    // Direita
    glVertex3f(largura, 0, 0);
    glVertex3f(largura, altura, 0);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(largura, 0, profundidade);

    glEnd();

    glPopMatrix();

    glTranslatef(0.0f,1.0f,0.0f);
    DesenhaBraco2();

}

void DesenhaCanhao()
{
    //- 3x2, formado por um paralele�pedo
    //- deve ter 2 atiradores
    //- velocidade = 2.5m/s
    //- possivel aumentar e diminuir forca de lancamento de projetil

    glPushMatrix();

    // Define as dimensões da base do canhao
    float largura = 3.0f;
    float altura = 1.0f;
    float profundidade = 2.0f; // Defina a profundidade conforme necessário


    // Posiciona o canhão no ponto de início
    glTranslated(posicaoCanhao.x,posicaoCanhao.y ,posicaoCanhao.z);

    //rotaciona na base
    glTranslated(0,0,profundidade/2);
    glRotatef(anguloBaseCanhao,0,1,0);
    glTranslated(0,0,-profundidade/2);

    // Define a cor do canhão para cinza
    glColor3f(0.5f, 0.5f, 0.5f); // RGB para cinza


    // Desenha o paralelepípedo (canhão)
    glBegin(GL_QUADS);

    // Topo
    glVertex3f(0, altura, 0);
    glVertex3f(0, altura, profundidade);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(largura, altura, 0);

    // Base
    glVertex3f(0, 0, 0);
    glVertex3f(largura, 0, 0);
    glVertex3f(largura, 0, profundidade);
    glVertex3f(0, 0, profundidade);

    // Frente
    glVertex3f(0, 0, profundidade);
    glVertex3f(largura, 0, profundidade);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(0, altura, profundidade);

    // Traseira
    glVertex3f(0, 0, 0);
    glVertex3f(0, altura, 0);
    glVertex3f(largura, altura, 0);
    glVertex3f(largura, 0, 0);

    // Esquerda
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, profundidade);
    glVertex3f(0, altura, profundidade);
    glVertex3f(0, altura, 0);

    // Direita
    glVertex3f(largura, 0, 0);
    glVertex3f(largura, altura, 0);
    glVertex3f(largura, altura, profundidade);
    glVertex3f(largura, 0, profundidade);

    glEnd();

    glTranslatef(2.5f, 1.0f, 0.75f); // Ajuste a posição Y conforme necessário
    DesenhaBraco1();

    glPopMatrix();
}

void DesenhaProjetil()
{
    /*
        - esfera deve ser do tamanho do canhao
        - movimento deve seguir formula da fisica de lancamento de projeteis

        - ao atingir um objeto, ganha ou perde pontos:

        - Objeto inimigo: +10 pontos
        - Objeto amigo: -10 pontos
        - Piso: -5 pontos
        - Paredao: +5 pontos
        - Canhao: fim do jogo
    */

    // Desenha o projétil
    glPushMatrix(); // Push the current matrix stack
    glTranslatef(posicaoProjetil.x, posicaoProjetil.y, posicaoProjetil.z); // Translate to the projectile's position
    glColor3f(1.0, 0.0, 0.0); // Set color to red for the projectile
    glutSolidSphere(0.25, 20, 20); // Draw a solid sphere with radius 0.25 and 20 slices and 20 stacks
    glPopMatrix(); // Pop the current matrix stack

}
/*
void AtiraProjetil(const Ponto& origem, const Ponto& destino, std::function<void()>&& onColisao) {
    // Calcula a trajetória usando curvas de Bézier
    std::vector<Ponto> trajetoria = CalculaCurvaBezier(origem, destino);

    // Itera sobre os pontos da trajetória
    for (const auto& ponto : trajetoria) {
        // Atualiza a posição do projetil
        meuProjetil.x = ponto.x;
        meuProjetil.y = ponto.y;
        meuProjetil.z = ponto.z;

        // Verifica colisões
        if (ColisaoProjetilObjetoAlvo(meuProjetil, meuAlvo)) {
            // Chama a função de callback em caso de colisão
            onColisao();
        }

        // Atualiza a tela
        glutPostRedisplay();
    }
}

std::vector<Ponto> CalculaCurvaBezier(const Ponto& origem, const Ponto& destino) {
    std::vector<Ponto> trajetoria;
    int numPontos = 100; // Número de pontos na trajetória

    // Gera os parâmetros t para os pontos da curva
    std::vector<float> parametros(numPontos + 1);
    std::iota(parametros.begin(), parametros.end(), 0.0f);
    std::transform(parametros.begin(), parametros.end(), parametros.begin(),
                   [numPontos](float t) { return t / numPontos; });

    // Calcula os pontos da curva de Bézier
    std::transform(parametros.begin(), parametros.end(), std::back_inserter(trajetoria),
                   [origem, destino](float t) {
                       return Ponto{
                           (1 - t) * origem.x + t * destino.x,
                           (1 - t) * origem.y + t * destino.y,
                           (1 - t) * origem.z + t * destino.z
                       };
                   });

    return trajetoria;
}
*/

// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    DefineLuz();
    PosicUser();

	glMatrixMode(GL_MODELVIEW);

    // comentado para piso ter textura glDisable(GL_TEXTURE_2D);
    DesenhaPiso();
    DesenhaParede();
    DesenhaCanhao();

    //glTranslatef(10,2,6);
    //DesenhaProjetil();

    if (ComTextura)
        glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);

    if (glIsEnabled(GL_TEXTURE_2D))
        glColor3f(1,1,1);
    else glColor3f(0.5, 0, 0);

    if (glIsEnabled(GL_TEXTURE_2D))
        glColor3f(1,1,1);
    else glColor3f(0.5f,0.0f,0.0f); // Vermelho

	//glPushMatrix();
      //glTranslatef ( 5.0f, 0.0f, 3.0f );
        //glRotatef(AngY,0,1,0);
        //glBindTexture (GL_TEXTURE_2D, TEX1);
		//DesenhaCubo();
	//glPopMatrix();

    if (glIsEnabled(GL_TEXTURE_2D))
        glColor3f(1,1,1);
    else glColor3f(0.6156862745, 0.8980392157, 0.9803921569); // Azul claro

	//glPushMatrix();
        //glTranslatef ( -4.0f, 0.0f, 2.0f );
		//glTranslatef ( 2.0f, 0f, 0.0f );
		//glRotatef(AngY,0,1,0);
        //glBindTexture (GL_TEXTURE_2D, TEX2);
		//DesenhaCubo();
	//glPopMatrix();

	glutSwapBuffers();
}

// **********************************************************************
//  void animate ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void animate()
{
    // Anima cubos
    AngY++;

    // Redesenha
    glutPostRedisplay();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 27:        // Termina o programa quando a tecla ESC for pressionada
            exit ( 0 );
            break;
        case 'W':
        case 'w':
            posicaoCanhao.x += velocidadeCanhao * direcaoVeiculo.x;
            posicaoCanhao.z += velocidadeCanhao * direcaoVeiculo.z;
            cout << "Nova posicao = " << posicaoCanhao.x << ", " << posicaoCanhao.z << endl;
            posicaoInicialProjetil = Ponto (posicaoCanhao.x + 3, posicaoCanhao.y + 1.5f, posicaoCanhao.z + 1);
            break;
        case 'A':
        case 'a': // Rotaciona o canhão para a esquerda
            anguloBaseCanhao += velocidadeRotacao;
            anguloBaseCanhao = fmod(anguloBaseCanhao, 360.0f); // Normaliza o ângulo
            if (anguloBaseCanhao < 0) anguloBaseCanhao += 360.0f;
            direcaoVeiculo = Ponto(1, 0, 0); // Resetar a direção original
            direcaoVeiculo.rotacionaY(anguloBaseCanhao); // Aplica a rotação total
            posicaoInicialProjetil = Ponto (posicaoCanhao.x + 3, posicaoCanhao.y + 1.5f, posicaoCanhao.z + 1);
            cout << "angulo: " << anguloBaseCanhao << endl;
            break;
        case 'D':
        case 'd': // Rotaciona o canhão para a direita
            anguloBaseCanhao -= velocidadeRotacao;
            anguloBaseCanhao = fmod(anguloBaseCanhao, 360.0f); // Normaliza o ângulo
            if (anguloBaseCanhao < 0) anguloBaseCanhao += 360.0f;
            direcaoVeiculo = Ponto(1, 0, 0); // Resetar a direção original
            direcaoVeiculo.rotacionaY(anguloBaseCanhao); // Aplica a rotação total
            posicaoInicialProjetil = Ponto (posicaoCanhao.x + 3, posicaoCanhao.y + 1.5f, posicaoCanhao.z + 1);
            cout << "angulo: " << anguloBaseCanhao << endl;
            break;
        case 'Q':
        case 'q':
            // Movimenta articulação 1
            anguloBraco1 -= velocidadeRotacao;
            break;
        case 'E':
        case 'e':
            // Movimenta articulação 2
            anguloBraco2 -= velocidadeRotacao;
            break;
        case 'T':
        case 't': // Atira o projetil
            //AtiraProjetil(); // Certifique-se de implementar esta função
            break;
        case 'C':
        case 'c': // Ativa/desativa textura
            ComTextura = !ComTextura;
            break;
        default:
            cout << key;
    break;
    }
    glutPostRedisplay();
}


// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 );
			break;
		default:
			break;
	}
}


struct BoundingBox {
    float largura, altura, profundidade;
    float x, y, z; // Posição da bounding box (centro)
};

struct Projetil {
    float raio; // Raio da esfera representando o projetil
    float x, y, z; // Posição do projetil
    BoundingBox boundingBox; // Bounding box do projetil
};

struct ObjetoAlvo {
    BoundingBox boundingBox; // Bounding box do objeto alvo
};

bool ColisaoBoundingBox(const BoundingBox& bb1, const BoundingBox& bb2) {
    // Verifica colisão nos eixos X, Y e Z
    bool colisaoX = std::abs(bb1.x - bb2.x) < (bb1.largura + bb2.largura) / 2.0f;
    bool colisaoY = std::abs(bb1.y - bb2.y) < (bb1.altura + bb2.altura) / 2.0f;
    bool colisaoZ = std::abs(bb1.z - bb2.z) < (bb1.profundidade + bb2.profundidade) / 2.0f;

    // Se houver colisão nos três eixos, então há colisão total
    return colisaoX && colisaoY && colisaoZ;
}

bool ColisaoProjetilObjetoAlvo(const Projetil& projetil, const ObjetoAlvo& alvo) {
    // Verifica a colisão entre a bounding box do projetil e a bounding box do objeto alvo
    return ColisaoBoundingBox(projetil.boundingBox, alvo.boundingBox);
}
// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 700 );
	glutCreateWindow    ( "Jogo do Paredao" );

	init ();
    //system("pwd");
    Projetil meuProjetil = {1.0f, 0.0f, 0.0f, 0.0f};
    meuProjetil.boundingBox = {1.0f, 1.0f, 1.0f, meuProjetil.x, meuProjetil.y, meuProjetil.z};

    ObjetoAlvo meuAlvo = {{2.0f, 2.0f, 2.0f, 3.0f, 0.0f, 0.0f}};

    if (ColisaoProjetilObjetoAlvo(meuProjetil, meuAlvo)) {
        std::cout << "Houve colisao entre o projetil e o objeto alvo.\n";
        // Faça algo aqui...
    } else {
        std::cout << "Nao houve colisao entre o projetil e o objeto alvo.\n";
    }
	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );
	return 0;
}
