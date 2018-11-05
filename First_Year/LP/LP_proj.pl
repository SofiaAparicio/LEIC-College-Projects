/*Grupo 102
Joana Godinho nÂº81478;
Ana Sofia Costa nÂº81105;*/

%%%-------------------------Resolve Manual------------------------------

resolve_manual(L1,L2):-transformacao_possivel(L1, L2),
					   write('Transformacao desejada:'),nl,
					   imprime_tabuleiros(L1,L2),jogo(L1,L2),!.

%%%imprime_tabuleiros/2
%%%imprime_tabuleiros([X1,X2,X3,X4,X5,X6,X7,X8,X9],[Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8,Y9])
%%% em que X1, X2,etc sao elementos da configuracao inicial e Y1,Y2,etc sao elementos da configuracao final
imprime_tabuleiros([X1,X2,X3,X4,X5,X6,X7,X8,X9],[Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8,Y9]):-imprime_linha(X1,X2,X3),write('    '),
																			   imprime_linha(Y1,Y2,Y3),nl,
																	           imprime_linha(X4,X5,X6),write(' -> '),
																			   imprime_linha(Y4,Y5,Y6),nl,
																	           imprime_linha(X7,X8,X9),write('    '),
																			   imprime_linha(Y7,Y8,Y9).

%%%imprime_linha/3, predicado aulixiar do imprime_tabuleiros/2
%%%imprime_linha(X,P,W) em que X,P e W sao inteiros
imprime_linha(X,P,W):-imprime(X),imprime(P),imprime(W).

%%%imprime/1 e um predicado auxiliar do imprime_linha/3
%%%imprime(X) em que X e um inteiro
imprime(X):-X=\=0,write(' '),write(X),write(' ').
imprime(X):-X==0,write('   ').


%%%imprime_tab/1
%%%imprime_tab([X1,X2,X3,X4,X5,X6,X7,X8,X9]) em que X1,X2,etc sao inteiros
%%%que constituem uma dada configuracao.
imprime_tab([X1,X2,X3,X4,X5,X6,X7,X8,X9]):- nl,imprime_linha(X1,X2,X3),nl,
											imprime_linha(X4,X5,X6),nl,
											imprime_linha(X7,X8,X9),nl.


%%%jogo/2 e um predicado auxiliar do resolve_manual
%%%jogo(L1,L2) em que L1 e L2 sao configuracoes representadas por listas.
jogo(L1,L1):-nl,write('Parabens!'),nl,!.
jogo(L1,L2):-nl,writeln('Qual o seu movimento?'),read(X),
			 mov_legal(L1,X,_,C2),imprime_tab(C2),
			 jogo(C2,L2),!.
jogo(L1,L2):-write('Movimento ilegal'),jogo(L1,L2).


%%%mov_legal/4 e um predicado auxiliar do jogo/2, do resolve_cego e do nos_possiveis
%%%mov_legal(C1,M,P,C2) em que C1 e C2 sao configuracoes,
%%%M e um carater relativo ao movimento e P e um inteiro(a peca)
mov_legal(C1,M,P,C2):-procura_zero(C1,Pos),mov_legal(C1,M,P,C2,Pos).
mov_legal(C1,c,P,C2,Pos):- Pos<6, Pos_p is Pos+3, escolhe(C1,Pos_p,P), altera(C1,P,C2).
mov_legal(C1,b,P,C2,Pos):- Pos>2, Pos_p is Pos-3, escolhe(C1,Pos_p,P), altera(C1,P,C2).
mov_legal(C1,e,P,C2,Pos):- Pos mod 3 < 2, Pos_p is Pos+1, escolhe(C1,Pos_p,P), altera(C1,P,C2).
mov_legal(C1,d,P,C2,Pos):- Pos mod 3 > 0, Pos_p is Pos-1, escolhe(C1,Pos_p,P), altera(C1,P,C2).

%%%procura_zero/2 e um predicado auxiliar ao mov_legal
%%%procura_zero(L,Pos) em que L Ã© uma configuracao (lista) e Pos
%%%e a posicao onde se encontra o 0.
procura_zero(L,Pos):- procura_zero(L,Pos,0).
procura_zero([0|_],Pos,Pos):- !.
procura_zero([_|R],Pos,Aux):- Aux2 is Aux+1,procura_zero(R,Pos,Aux2).


%%%escolhe/3 e um predicado auxiliar ao mov_legal
%%%escolhe([_|R],Pos_p,P) em que recebe uma configuracao(lista) e uma posicao (inteiro)
%%%e indica que peca esta nessa posicao (P).
escolhe([_|R],Pos_p,P):- Pos_p=\=0,Pos_p2 is Pos_p-1,
						 escolhe(R,Pos_p2,P).
escolhe([X|_],0,P):- P=X.


%%%altera/3 e um predicado auxiliar ao mov_legal
%%%altera([X|R],P,[X|R1]) em que [X|R] e uma configuraÃ§ao e [X|R1]
%%%e a configuracao apos se trocar o membro P com 0 e vice-versa
altera([],_,[]) :- !.
altera([P|R],P,[0|R1]):- altera(R,P,R1).
altera([0|R],P,[P|R1]):- altera(R,P,R1).
altera([X|R],P,[X|R1]):-X=\=0, X=\=P, altera(R,P,R1).


%%%-------------------------Resolve Cego------------------------------

%%%resolve_cego/2
resolve_cego(L1,L1):-write('Transformacao desejada:'),nl,
					imprime_tabuleiros(L1,L1),nl,!.
resolve_cego(L1,L2):-transformacao_possivel(L1, L2),
					 write('Transformacao desejada:'),nl,
					 imprime_tabuleiros(L1,L2),nl,
					 resolve_cego(L1,L2,[L1],[]),!.

%%%resolve_cego/4 e um predicado auxiliar de resolve_cego/2
%%%resolve_cego(L1,L2,Lconf,L_jogo) em que L1 e L2 sao configuracao,
%%% Lconf e a lista com as configuracao anteriores e L_jogo e a lista com os movimentos
resolve_cego(L1,L1,_,L_jogo):- inverte_lista(L_jogo,[],L_jogo_inv),
							   escreve_jogo(L_jogo_inv),!.

resolve_cego(L1,L2,Lconf,L_jogo):- L1\==L2, mov_legal(L1,M,P,C),
								   configuracao_n_existe(C,Lconf),
								   NovaLconf=[C|Lconf],
								   Nova_Ljogo=[[M,P]|L_jogo],
								   resolve_cego(C,L2,NovaLconf,Nova_Ljogo).

%%%inverte_lista/3 e um predicado auxiliar ao resolve_cego/4 e ao resolve_info_h
%%%inverte_lista([P|R],Aux,I) em que recebe uma lista [P|R] e devolve a lista invertida
inverte_lista([],I,I):-!.
inverte_lista([P|R],Aux,I):-inverte_lista(R,[P|Aux],I).

%%%configuracao_n_existe/2 e um predicado auxiliar para a resolve_cego/4
%%%configuracao_n_existe(Cf,[P|R]) em que Cf e uma configuracao e o ([P|R]) e uma lista de
%%%configuracoes ja existentes
configuracao_n_existe(_,[]).
configuracao_n_existe(Cf,[P|R]):- Cf\==P,configuracao_n_existe(Cf,R).

%%%escreve_jogo/1 e um predicado auxiliar de resolve_cego/4
%%% escreve_jogo([[M,P]|[]]) imprime a lista de movimentos
escreve_jogo([[M,P]|[]]):-escreve_jogada(M,P),writeln('.'),!.
escreve_jogo([[M,P]|R]):-escreve_jogada(M,P),nl,escreve_jogo(R).

%%%escreve_jogada/2 e um predicado auxiliar de escreve_jogo/1
%%%escreve_jogada(M,P) imprimeo movimento (M) feito pela peca (P)
escreve_jogada(b,P):-write('mova a peca '),write(P),write(' para baixo').
escreve_jogada(c,P):-write('mova a peca '),write(P),write(' para cima').
escreve_jogada(e,P):-write('mova a peca '),write(P),write(' para a esquerda').
escreve_jogada(d,P):-write('mova a peca '),write(P),write(' para a direita').

%%%-------------------------Resolve Informada------------------------------

%%%cria_no/6 cria uma estrutura que e um no
%%%cria_no(C,F,G,H,M,no(C,F,G,H,M)) em que o C e a configuracao, F e o valor da
%%% funcao_f, G e o numero movimentos feitos, H e o numero de hamming e M e a
%%%lista com os movimentos
cria_no(C,F,G,H,M,no(C,F,G,H,M)).
configuracao(no(C,_,_,_,_),C).
funcao_f(no(_,F,_,_,_),F).
funcao_g(no(_,_,G,_,_),G).
funcao_h(no(_,_,_,H,_),H).
movimentos(no(_,_,_,_,M),M).


%%%resolve_info_h/2
resolve_info_h(L1,L1):- writeln('Transformacao desejada:'),
						imprime_tabuleiros(L1,L1),nl,!.
resolve_info_h(L1,L2):- transformacao_possivel(L1, L2),
						writeln('Transformacao desejada:'),
						imprime_tabuleiros(L1,L2),nl,
						distancia_hamming(L1,L2,0,H), F1 is 0+H,
						cria_no(L1,F1,0,h,[],No),
						resolve_info_h(L2,[No],[]),!.

%%%resolve_info_h/3 e uma funcao auxiliar de resolve_info_h/3
%%%resolve_info_h(Cfinal,L_abertos,L_fechados) em que Cfinal e ??????
%%%L_abertos e a lista de abertos e L_fechados e a lista de fechados
resolve_info_h(_,[],_):-writeln('Nao existe solucao'),!.

resolve_info_h(Cfinal,L_abertos,L_fechados):- procura_no(L_abertos,No),
											  trata_nos(Cfinal,L_abertos,L_fechados,No),!.


trata_nos(_,_,_,No):-funcao_h(No,H),H==0,movimentos(No,M),
					 inverte_lista(M,[],Mfinal),
					 escreve_jogo(Mfinal),!.


%%%trata_nos/4 e a funcao auxiliar de resolve_info_h/2
%%%trata_nos(Cfinal,L_abertos,L_fechados,No) em que Cfinal e a configuracao pretendida
%%% L_abertos a lista de abertos e L_fechados a lista de fechados
trata_nos(Cfinal,L_abertos,L_fechados,No):- remove_no(L_abertos,No,L_aberNova),
											L_fechados1=[No|L_fechados],
											expande_no(No,L_abertos,L_fechados,Cfinal,S),
											inverte_lista(S,[],Sinvertida),
											adiciona_sucessores(Sinvertida,L_aberNova,L_aberNova1),
											resolve_info_h(Cfinal,L_aberNova1,L_fechados1).


%%%procura_no/2 e uma funcao auxiliar de resolve_info_h/3
procura_no([P|R],Menor):-funcao_f(P,F),procura_no(R,F,P,Menor).

%%%procura_no/3 e uma funcao auxiliar de procura_no/3
%%%procura_no([P|R],Aux_f,Aux_No,Menor) em que ([P|R]) e a lista, Aux_f e uma variavel auxiliar
%%%Aux_No e uma variavel auxiliar e Menor o no com menor F
procura_no([],_,Menor,Menor):-!.
procura_no([P|R],Aux_f,_,Menor):-funcao_f(P,F),F < Aux_f,
								 Aux_f1 = F,Aux_No=P,
								 procura_no(R,Aux_f1,Aux_No,Menor),!.
procura_no([_|R],Aux_f,Aux_No,Menor):-procura_no(R,Aux_f,Aux_No,Menor).


%%%remove_no/3 e uma funcao auxiliar de trata_nos/4
%%%remove_no([P|R],E,L) em que [P|R] e uma lista, E e o elemento a remover
%%% e L e a lista sem o elemento E
remove_no([],_,_).
remove_no([N|R],E,L):- N==E,remove_no(R,E,L),!.
remove_no([P|R],E,L):-remove_no(R,E,Aux1),L=[P|Aux1].


%%%expande_no/5 e uma funcao auxiliar do trata_nos
%%%expande_no(N_inicial,L1,L2,C_final,S) em que N_inicial e o no a ser expandido
%%% L1 e L2 sao duas listas de nos, C_final e a configuracao final
%%%e S e o conjunto de nos sucessores a N_inicial que nao existem em L1 e em L2.
expande_no(N_inicial,L1,L2,C_final,S):- findall(N,nos_possiveis(N_inicial,L1,L2,C_final,N),S),!.


%%%nos_possiveis/5 e uma funcao auxiliar de expande_no/5
%%%nos_possiveis(N,L1,L2,Cfinal,No) em que N e o no a expandir, L1 e L2 sao
%%%listas de nos, Cfinal configuraÃ§ao final e No um no sucessor a N
nos_possiveis(N,L1,L2,Cfinal,No):- configuracao(N,Ci),funcao_g(N,Gi),
								   mov_legal(Ci,M,P,C2),
								   distancia_hamming(C2,Cfinal,0,H),
								   funcao_g(N,Gi),
								   G is Gi+1, F is G+H,
								   movimentos(N,Movs),Movs_novo=[[M,P]|Movs],
							       cria_no(C2,F,G,H,Movs_novo,N_possivel),
								   no_n_existe(N_possivel,L1),
								   no_n_existe(N_possivel,L2),No=N_possivel.


%%%no_n_existe/2 e uma funcao auxiliar de nos_possiveis
%%%no_n_existe(N,[P|L1]) em que N e um no e [P|L1] e uma lista de nos
%%%verifica se N existe em [P|L1].
no_n_existe(_,[]):-	!.
no_n_existe(N,[P|L1]):-configuracao(N,Cn),
					   configuracao(P,Ce),Cn\==Ce,
					   no_n_existe(N,L1).

%%%adiciona_sucessores/3 e uma funcao auxiliar de trata_nos/4
%%% adiciona_sucessores([P|R],L,Lf) em que [P|R] e L sao uma lista de nos,
%%% Lf e a juncao entre [P|R] e L
adiciona_sucessores([],Lf,Lf):-!.
adiciona_sucessores([P|R],L,Lf):-L2=[P|L],adiciona_sucessores(R,L2,Lf).


%%%distancia_hamming/4 e uma funcao auxiliar de nos_possiveis/5
%%%distancia_hamming([P|R],[E|R1],Aux,Cont)	em que [P|R] e uma certa configuracao,
%%%[E|R1] e a configuracao pretendida, Aux e uma variavel auxiliar e Cont e a distancia de hamming
distancia_hamming([],[],Cont,Cont).
distancia_hamming([P|R],[E|R1],Aux,Cont):- P \== E,P\==0,!,
											Aux1 is Aux + 1,
										   distancia_hamming(R,R1,Aux1,Cont).
distancia_hamming([_|R],[_|R1],Aux,Cont):- distancia_hamming(R,R1,Aux,Cont).



%%%-------------------------Transformacao Possivel------------------------------
%%%transformacao_possivel/2
transformacao_possivel(C1, C2):- conta_inversoes(C1, Cont),
			 conta_inversoes(C2,Cont2), Cont mod 2 =:= Cont2 mod 2,!.

%%%conta_inversoes/2 e uma funcao auxiliar de transformacao_possivel/2
conta_inversoes(C1, Cont):-conta_inversoes(C1,Cont,0,[]).

%%%conta_inversoes/4 e uma funcao auxiliar de conta_inversoes/2
%%%conta_inversoes([P1|R1],Cont,Aux,Tras) em que [P1|R1] e uma lista(tabuleiro),
%%%Cont conta o numero de inversoes , Aux e uma variavel auxiliar e Tras e a lista de elementos ja anal
conta_inversoes([],Cont,Cont,_).
conta_inversoes([P1|R1],Cont,Aux,Tras):- P1=\=0, ver_tras(P1,Tras,Inc),
										Nova_tras = [P1|Tras],
										Aux1 is Aux + Inc,
										conta_inversoes(R1,Cont,Aux1,Nova_tras).
conta_inversoes([0|R1],Cont,Aux,Tras):- conta_inversoes(R1,Cont,Aux,Tras).


%%%ver_tras/3 e uma funcao auxiliar de conta_inversoes/3
ver_tras(P,Tras,Inc):- ver_tras(P,Tras,Inc,0).

%%%ver_tras/4 e uma funcao auxiliar de ver_tras/3
%%%ver_tras(P,[P1|R1],Inc, Aux) em que P e o numero a comparar, [P1|R1] a lista de
%%%numeros anteriores, Inc e o numero de elementos da lista que e maior do
%%%que P(numero de inversoes) e Aux e uma variavel auxiliar
ver_tras(_,[],Inc,Inc):-!.
ver_tras(P,[P1|R1],Inc, Aux):- P=<P1,Aux1 is Aux+1,
							   ver_tras(P,R1,Inc,Aux1),!.
ver_tras(P,[P1|R1],Inc,Aux):- P>=P1,ver_tras(P,R1,Inc,Aux).
