#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//gcc -o macrode.exe macrode.c -luser32 -lgdi32

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND hwndTextbox1,hwndTextbox2,hwndButton ;
void enviarTeclaDeVolta(int i) {
    // Simular a pressionar da tecla Enter
    keybd_event(i, 0, 0, 0);

    // Simular o soltar da tecla Enter
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}
void enviarStringDeVolta(const char* string) {
    // Percorrer cada caractere da string
    int ii=strlen(string);
    for (int i = 0; i<ii; i++) {
        // Simular a pressionar da tecla correspondente a cada caractere
        keybd_event(string[i], 0, 0, 0);

        // Simular o soltar da tecla correspondente a cada caractere
        keybd_event(string[i], 0, KEYEVENTF_KEYUP, 0);
    }
}
void moverCursor(int x, int y) {
    POINT point;
    point.x = x;
    point.y = y;

    SetCursorPos(point.x, point.y);
}
void abrirEmSegundoPlano(const char*  frase) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    // Criar uma nova janela oculta para o processo 
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    // Abrir o Notepad em modo maximizado
    if (CreateProcess(frase, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        // Fechar os handles do processo e da thread
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        printf("Erro ao abrir .\n");
    }
}
void macrodesponto(const char*  frase) {
    // Implemente a lógica da função macrodes aqui
     // Implemente a lógica da função macrodes aqui
    char copiaFrase[1024];
    int count =0;
    int args[100];
    
    strcpy(copiaFrase, frase);
    // Definir o texto do campo de texto 2
    SetWindowText(hwndTextbox2, frase);
    char* token[1024];
    token[0] = strtok(copiaFrase, ",");
    
    while (token[count] != NULL) {
        //printf("%s\n", token);
	count++;
        token[count] = strtok(NULL, ",");
    }
    if(strcmp(token[0],"sleep")==0){
    	args[0]= atoi(token[1]);
	if (count>1)Sleep(args[0]);  // Converte segundos para milissegundos
    }
if(strcmp(token[0],"run")==0){
	if (count>1)abrirEmSegundoPlano(token[1]);  // Converte segundos para milissegundos
    }
if(strcmp(token[0],"xy")==0){
	if (count>2)moverCursor(atoi(token[1]), atoi(token[2]));  // Converte segundos para milissegundos
    }
if(strcmp(token[0],"print")==0){
	if (count>1)enviarStringDeVolta (token[1]);  
    }
 if(strcmp(token[0],"key")==0){
    	args[0]= atoi(token[1]);
	if (count>1)enviarTeclaDeVolta(args[0]);  // Converte segundos para milissegundos
    }
    
}
void macrodes(const char* frase) {
    // Implemente a lógica da função macrodes aqui
    char copiaFrase[1024];
    strcpy(copiaFrase, frase);
    
    char* token = strtok(copiaFrase, ";");
    
    while (token != NULL) {
        macrodesponto(token);
        token = strtok(NULL, ";");
    }




}

void processarArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    char linha[256];
    
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Remover o caractere de nova linha
        if (linha[strlen(linha) - 1] == '\n') {
            linha[strlen(linha) - 1] = '\0';
        }
        
        // Chamar a função macrodes para cada linha
        macrodes(linha);
    }
    
    fclose(arquivo);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Registrar a classe da janela
    const char CLASS_NAME[] = "MinhaJanela";
    
    WNDCLASS wc = { 0 };
    
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClass(&wc);
    
    // Criar a janela
    HWND hwnd = CreateWindowEx(
        0,                      // Estilo estendido
        CLASS_NAME,             // Classe da janela
        "Minha Janela",         // Título da janela
        WS_OVERLAPPEDWINDOW,    // Estilo da janela
        CW_USEDEFAULT,          // Posição x da janela
        CW_USEDEFAULT,          // Posição y da janela
        800,                    // Largura da janela
        600,                    // Altura da janela
        NULL,                   // Janela pai
        NULL,                   // Menu
        hInstance,              // Instância
        NULL                    // Parâmetros adicionais
    );
    
    if (hwnd == NULL) {
        return 0;
    }
       // Criar o campo de texto 1
    hwndTextbox1 = CreateWindow(
        "EDIT",                 // Classe do campo de texto
        "",                     // Texto inicial
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,  // Estilo do campo de texto
        10,                     // Posição x do campo de texto
        10,                     // Posição y do campo de texto
        200,                    // Largura do campo de texto
        20,                     // Altura do campo de texto
        hwnd,                   // Janela pai
        NULL,                   // ID do campo de texto (nulo)
        hInstance,              // Instância
        NULL                    // Parâmetros adicionais
    );
 // Criar o campo de texto 2
    hwndTextbox2 = CreateWindow(
        "EDIT",                 // Classe do campo de texto
        "",                     // Texto inicial
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_READONLY,  // Estilo do campo de texto (somente leitura)
        10,                     // Posição x do campo de texto
        40,                     // Posição y do campo de texto
        200,                    // Largura do campo de texto
        20,                     // Altura do campo de texto
        hwnd,                   // Janela pai
        NULL,                   // ID do campo de texto (nulo)
        hInstance,              // Instância
        NULL                    // Parâmetros adicionais
    );
// Criar o botão
    hwndButton = CreateWindow(
        "BUTTON",               // Classe do botão
        "OK",                   // Texto do botão
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Estilo do botão
        220,                    // Posição x do botão
        10,                     // Posição y do botão
        60,                     // Largura do botão
        50,                     // Altura do botão
        hwnd,                   // Janela pai
        NULL,                   // ID do botão (nulo)
        hInstance,              // Instância
        NULL                    // Parâmetros adicionais
    );

    // Exibir a janela
    ShowWindow(hwnd, nCmdShow);
    
    // Loop de mensagem da janela
    MSG msg = { 0 };
    
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
 case WM_COMMAND:
            if (LOWORD(wParam) == BN_CLICKED && (HWND)lParam == hwndButton) {
                // Obter o texto do campo de texto 1
                char buffer[256];
                GetWindowText(hwndTextbox1, buffer, sizeof(buffer));
                
                // Concatenar ".bat" ao texto
                strcat(buffer, ".bat");
                processarArquivo(buffer);
                
                
                return 0;
            }
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Pintar a janela de azul
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
