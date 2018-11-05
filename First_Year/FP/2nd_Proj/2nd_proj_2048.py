#------------------------------------------------------------------------------
#                                 TAD coordenada
#------------------------------------------------------------------------------
def cria_coordenada(linha,coluna):
    """int x int -> tuple
    Esta funcao recebe duas coordenadas do tipo inteiro, a primeira correspondente a linha e a segunda a coluna e devolve um elemento do tipo coordenada, ou seja, o tuplo (linha,coluna)"""
    if 1<=linha<=4 and 1<=coluna<=4 and isinstance(linha,int) and isinstance(coluna,int):
        return (linha,coluna)
    raise ValueError('cria_coordenada: argumentos invalidos')

def coordenada_coluna(coordenada):
    """tuplo -> int
    Esta funcao recebe um tuplo (elemento do tipo coordenada) e devolve a coluna respetiva"""
    return coordenada[1]

def coordenada_linha(coordenada):
    """tuplo -> int
       Esta funcao recebe um tuplo (elemento do tipo coordenada) e devolve a linha respetiva"""    
    return coordenada[0]

def e_coordenada(arg):
    """tuplo -> Boole
    Esta funcao verifica se o argumento que recebe e um tuplo do tipo coordenada"""
    return isinstance(arg,tuple) and len(arg)==2 and 1<=coordenada_linha(arg)<=4 and 1<=coordenada_coluna(arg)<=4 and isinstance(coordenada_linha(arg),int) and isinstance(coordenada_coluna(arg),int)

def coordenadas_iguais(c1, c2):
    """tuplo x tuplo -> Boole
    Esta funcao recebe dois tuplos do tipo coordenada e verifica se estas coordenadas sao ou nao iguais"""
    if e_coordenada(c1) and e_coordenada(c2):
        return c1==c2

#------------------------------------------------------------------------------
#                                 TAD Tabuleiro
#------------------------------------------------------------------------------
from random import random

def cria_tabuleiro():
    """-> tab
    A funcao nao recebe qualquer tipo de argumento, e devolve um elemento do tipo lista, correspondente ao tabuleiro de jogo"""
    tabuleiro=[]
    aux=[]
    for linha in range(0,4):
        for coluna in range(0,4):
            aux=aux+[0]
        tabuleiro=tabuleiro+[aux] #preenche progressivamente com 0, de acorde com o ciclo
        aux=[]
    tabuleiro=tabuleiro+[0]
    return tabuleiro

t=cria_tabuleiro()

def tabuleiro_posicao(t,c):
    """list x tuplo -> int
    Esta funcao recebe um tabuleiro, do tipo lista, e um elemento tuplo, do tipo coordenada,e devolve um elemento do tipo inteiro, que corresponde ao valor na posicao do tabuleiro da coordenada c"""
    if e_coordenada(c):
        return t[(coordenada_linha(c)-1)][(coordenada_coluna(c)-1)] #pois o indice das listas comeca em 0, e o das coordenadas em 1
    else:
        raise ValueError('tabuleiro_posicao: argumentos invalidos')
    
def tabuleiro_pontuacao(t):
    """dic -> int
    Esta funcao recebe um elemento tabuleiro, do tipo lista, e devolve a pontuacao atual para o tabuleiro analisado"""
    return t[4]

def tabuleiro_posicoes_vazias(t):
    """list -> list
    Esta funcao recebe um elemento tabuleiro do tipo lista e devolve uma lista que contem as coordenadas de todas as posicoes vazias desse tabuleiro"""
    lista=[]
    for linha in range(len(t)-1):
        for coluna in range(len(t[linha])):
            if t[linha][coluna]==0:
                lista=lista+[cria_coordenada(linha+1,coluna+1)] #pois o indice das listas comeca em 0, e o das coordenadas em 1
    return lista

def tabuleiro_preenche_posicao(t,c,v):
    """list x tuplo x int -> list
    Esta funcao recebe um elemento tabuleiro do tipo lista, um elemento coordenada, do tipo tuplo e um elemento v do tipo inteiro, e modifica o tabuleiro t, colocando o valor v na posicao correspondente a coordenada c """
    if e_coordenada(c) and isinstance(v,int):
        t[coordenada_linha(c)-1][coordenada_coluna(c)-1]=v  #pois o indice das listas comeca em 0
        return t
    else:
        raise ValueError('tabuleiro_preenche_posicao: argumentos invalidos')
    
def tabuleiro_actualiza_pontuacao(t,v):
    """list x int -> list
    Esta funcao recebe um elemento tabuleiro do tipo lista e um elemento v do tipo inteiro e modifica o tabuleiro, acrescentando ao valor da pontuacao v pontos"""
    if isinstance(v,int) and v%4==0 and v>=0:
        t[4]=tabuleiro_pontuacao(t)+v
        return t
    else:
        raise ValueError('tabuleiro_actualiza_pontuacao: argumentos invalidos')


#---------------REDUZ-----------------

c=['N','S','E','W']

def tabuleiro_reduz(t,d):
    """list x str -> list
    Esta funcao recebe como argumento uma lista do tipo tabuleiro e uma cadeia de caracteres correspondente a uma das quatro direccoes possiveis em que o tabuleiro pode ser reduzido. Assim, a funcao deve modificar o tabuleiro que esta recebe, reduzindo-o na direccao indicada, e devolvendo no fim o tabuleiro modificado"""
    if d not in c:
        raise ValueError('tabuleiro_reduz: argumentos invalidos')
    else:
        rang_i={'N': range(1,4), 'S': range(4,1,-1), 'E': range(4,1,-1) , 'W': range(1,5)}
        rang_j={'N': range(1,5), 'S': range(1,5), 'E': range(1,5), 'W': range(1,4)}          
        def puxa(t):          
            for w in range (1,4):
                for j in rang_j[d]:
                    for i in rang_i[d]:
                        coord_1={'N': cria_coordenada(i,j), 'S': cria_coordenada(i,j), 'E': cria_coordenada(j, i), 'W': cria_coordenada(i, j)}
                        coord_2a={'N': i+1, 'S': i-1, 'E': j, 'W': i} #as coord_2 foram separadas de modo a permitir a existencia de abstracao, recorrendo ao cria_coordenada. Caso estivesse como o coord_1, o dicionario iria gerar um erro ao tentar ser criado, visto que apenas com o range de uma das direcoes (escolhido previamente) nao e possivel criar todas aquelas coordenadas (algumas delas iriam ser argumentos invalidos para o cria_coordenada)
                        coord_2b={'N': j, 'S': j, 'E': i-1, 'W': j+1}
                        #Quando a direcao e Este, o j para a ser as linhas e o i as colunas devido ao modo como o ciclo deve percorrer o tabuleiro (trocar a ordem dos dois ranges acima)
                        if tabuleiro_posicao(t,coord_1[d])==0 and tabuleiro_posicao(t,cria_coordenada(coord_2a[d],coord_2b[d]))!=0:
                            tabuleiro_preenche_posicao(t,coord_1[d],tabuleiro_posicao(t,cria_coordenada(coord_2a[d],coord_2b[d])))  #como o numero era 0, o numero na posicao abaixo "sobe"
                            tabuleiro_preenche_posicao(t,cria_coordenada(coord_2a[d],coord_2b[d]),0)  #a posicao abaixo passa a zero para, se for o caso, o ciclo continuar 
            return t        
        
        def junta(t):
            for j in rang_j[d]:
                for i in rang_i[d]:
                    coord_1={'N': cria_coordenada(i,j), 'S': cria_coordenada(i,j), 'E': cria_coordenada(j, i), 'W': cria_coordenada(i, j)}
                    coord_2a={'N': i+1, 'S': i-1, 'E': j, 'W': i}
                    coord_2b={'N': j, 'S': j, 'E': i-1, 'W': j+1}
                    if tabuleiro_posicao(t,coord_1[d])==tabuleiro_posicao(t,cria_coordenada(coord_2a[d],coord_2b[d])):
                        tabuleiro_preenche_posicao(t,coord_1[d],2*(tabuleiro_posicao(t,cria_coordenada(coord_2a[d],coord_2b[d])))) #os dois numeros iguais somam-se
                        tabuleiro_preenche_posicao(t,cria_coordenada(coord_2a[d],coord_2b[d]),0)  #o numero e colocado a zero para o ciclo continuar, e o numero que se encontra abaixo deste poder "subir"
                        tabuleiro_actualiza_pontuacao(t,tabuleiro_posicao(t,coord_1[d]))
            return t        
        t=puxa(t)
        t=junta(t)
        t=puxa(t)            
    return t        
    
            
def e_tabuleiro(t):
    """list -> Boole
    Esta funcao recebe um dicionario, e devolve True caso esse dicionario seja um tabuleiro de jogo, e False caso nao seja"""
    if isinstance (t, list) and len(t)==5 and isinstance(tabuleiro_pontuacao(t), int):
        for l in range(4):
            for c in range(4):
                if not isinstance(t[l][c], int):
                    return False
            return True
    return False

def tabuleiros_iguais(t1,t2):
    """list, list -> Boole
    Esta funcao recebe duas listas do tipo tabuleiro e devolve True se os tabuleiros forem iguais, e False caso nao o sejam"""
    if e_tabuleiro(t1) and e_tabuleiro(t2):
        return t1==t2

def escreve_tabuleiro(t):
    """list -> 
    Esta funcao recebe uma lista do tipo tabuleiro e escreve para o ecra a representacao externa de um tabuleiro de 2048"""
    if e_tabuleiro(t):
        for i in range(1,5):
            print('[', tabuleiro_posicao(t,cria_coordenada(i,1)), ']','[', tabuleiro_posicao(t,cria_coordenada(i,2)), ']','[', tabuleiro_posicao(t,cria_coordenada(i,3)), ']','[', tabuleiro_posicao(t,cria_coordenada(i,4)), '] ') #os espacos foram dados para a matriz ter o formato que tem no enunciado
        print('Pontuacao:',tabuleiro_pontuacao(t))
    else:
        raise ValueError('escreve_tabuleiro: argumentos invalidos')
    
def pede_jogada():
    """->str
    Esta funcao nao recebe qualquer argumento, pedindo apenas ao utilizador para introduzir uma direcao(N, S, E, W) e, caso o argumento seja valido, devolve a cadeia de carateres correspondente a direcao introduzida"""
    x=str(input('Introduza uma jogada (N, S, E, W): ', ))
    if x in c:
        return x
    else:
        print('Jogada invalida')
        return pede_jogada()

def preenche_posicao_aleatoria(t):
    """list -> list
    Esta funcao recebe uma lista do tipo tabuleiro e preenche uma posicao livre (escolhida aleatoriamente) com um dos numeros 2 e 4, de acordo com as probabilidades ja indicadas"""
    x=tabuleiro_posicoes_vazias(t)
    v=x[int(random()*(len(x)-1))] #o indice da lista comeca em 0, e nao em 1 (o que aconteceria se fizesse apenas len(x))
    if (random()*10)>=8:
        return tabuleiro_preenche_posicao(t,v,4)
    else:
        return tabuleiro_preenche_posicao(t,v,2)

def copia_tabuleiro(t):
    """list -> list
    Esta funcao recebe uma lista do tipo tabuleiro e devolve uma copia do mesmo"""
    tabuleiro=[]
    aux=[]
    for l in range(1,5):
        for c in range(1,5):
            aux=aux+[tabuleiro_posicao(t,cria_coordenada(l,c))] #percorre o tabuleiro coordenada a coordenada
        tabuleiro=tabuleiro+[aux] #passa para a copia do tabuleiro o valor de cada posicao, coordenada a coordenada
        aux=[]
    tabuleiro=tabuleiro+[tabuleiro_pontuacao(t)]
    return tabuleiro

def tabuleiro_terminado(t):
    """list -> Boole
    Esta funcao recebe uma lista do tipo tabuleiro e indica se este esta terminado ou nao, ou seja, se ainda e ou nao possivel reduzir o tabuleiro"""
    if tabuleiro_posicoes_vazias(t)==[]:
        for l in range(1,5):
            for c in range(1,5):
                return not (tabuleiro_posicao(t,cria_coordenada(l,c))==tabuleiro_posicao(t,cria_coordenada(l+1,c)) and tabuleiro_posicao(t,cria_coordenada(l,c))==tabuleiro_posicao(t,cria_coordenada(l,c+1)))
    return False

def jogo_2048():
    """Esta funcao nao recebe qualquer argumento, e permite a um utilizador jogar um jogo completo de 2048"""
    tabuleiro=preenche_posicao_aleatoria(t)
    tabuleiro=preenche_posicao_aleatoria(t)
    escreve_tabuleiro(t)
    while not tabuleiro_terminado(t):
        copia=copia_tabuleiro(t)
        tabuleiro=tabuleiro_reduz(t,pede_jogada())
        if copia==tabuleiro:
            escreve_tabuleiro(t)
        else:
            tabuleiro=preenche_posicao_aleatoria(t) 
            escreve_tabuleiro(t)
    print('Game over. A sua pontuacao foi:', tabuleiro_pontuacao(t))