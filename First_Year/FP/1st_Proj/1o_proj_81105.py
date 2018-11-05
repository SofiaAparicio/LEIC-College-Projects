#81105 Sofia Costa

def digito_verificacao(y):
    ver = eval(y)%10
    return ver

def calc_soma( y ):
    """Inverte os digitos, duplica os impares (imp > 9, subtrai por 9 ), 
    adiciona numeros"""
    inv =''
    par = 0
    num = 0
    fin = 0
    i=len(y)-1
    while i>=0:
        inv += y[i]
        i=i-1
    for i in range(len(inv)):
        num = eval(inv[i])
        if (i+1)%2 != 0:
            num = num * 2
            if num >9:
                num = num - 9
        fin += num
    return fin

def luhn_verifica(w):
    ver = digito_verificacao(w)
    w = eval(w)//10
    soma = calc_soma(str(w)) + ver
    if soma % 10 == 0:
        return True
    else:
        return False
    
#def redes_cc(rede,iin,dig,ab): # red-rede emissora; iin-digitos inicias; dig-num de digitos
AE = ('America Express',('34','37'),'15','AE')
DCI = ('Diners Club International',('309','36','38','39'),'14','DCI')
DC = ('Discover Card','65','16','DC')
M = ('Maestro',('5018','5020','5038'),('13','19'),'M')
MC = ('Master Card',('50','54','19'),'16','MC')
VE = ('Visa Electron',('4026','426','4405','4508'),'16','VE')
V = ('Visa', ('4024', '4532', '4556'), ('13', '16'), 'V')
red = (AE[0],DCI[0],DC[0],M[0],MC[0],VE[0],V[0])
iin = (AE[1],DCI[1],DC[1],M[1],MC[1],VE[1],V[1])
dig = (AE[2],DCI[2],DC[2],M[2],MC[2],VE[2],V[2])
ab = (AE[3],DCI[3],DC[3],M[3],MC[3],VE[3],V[3])

def comeca_por(x,y):
    """Recebe duas cadeia de caracteres, sendo a primeira o numero do cartao e a segunda os digitos iniciais (IIN)"""
    if len(x)<len(y):
        return False
    for i in range(len(y)):
        if y[i]!=x[i]:
            return False
    return True

def comeca_por_um(c,t_cads):
    for i in range(len(t_cads)):
        if comeca_por(c,t_cads[i]):
            return True
    return False

def categoria(q):
    if comeca_por(q, '1') == True:
        return 'Companhias Aereas'
    elif comeca_por(q, '2') == True:
        return 'Companhias aéreas e outras tarefas futuras da indústria' 
    elif comeca_por(q, '3') == True:
        return 'Viagens e entretenimento e bancário / financeiro'
    elif comeca_por(q, '4') or comeca_por(q, '5'):
        return 'Serviços bancários e financeiros' 
    elif comeca_por(q, '6') == True:
        return 'Merchandising e bancário / financeiro'   
    elif comeca_por(q, '7') == True:
        return 'Petróleo e outras atribuições futuras da indústria'
    elif comeca_por(q, '8') == True:
        return 'Saúde, telecomunicações e outras atribuições futuras da indústria'    
    elif comeca_por(q, '9') == True:
        return 'Atribuição nacional'
    else: return ''

def valida_iin(c):
    if len(c) == 19:
        if comeca_por(c, M[1]) == True:
            return M[0]
    elif len(c) == 16:
        if comeca_por(c, DC[1]) == True:
            return DC[0]
        elif comeca_por_um(c, MC[1]) == True:
            return MC[0]   
        elif comeca_por_um(c, VE[1]) == True:
            return VE[0] 
        elif comeca_por_um(c, V[1]) == True:
            return V[0]
    elif len(c) == 15:
        if comeca_por(c, AE[1]) == True:
            return AE[0]
    elif len(c) == 14:
        if comeca_por(c, DCI[1]) == True:
            return DCI[0]
    elif len(c) == 13:
        if comeca_por(c, M[1]) == True:
            return M[0]
        if comeca_por(c, V[1]) == True:
            return V[0]
    return ''

def verifica_cc(x):
    n=str(x)
    if luhn_verifica(n) == True:
        return(categoria(n), valida_iin(n))
    else: return'cartao invalido'
    
def gera_num_cc(cart):
    if cart == 'AE':
        x = AE[1][random(len(AE[1]))]
        y = eval(AE[2])-(len(x)-1)
    elif cart == 'DCI':
        x = DCI[1][random(len(DCI[1]))]
        y = eval(DCI[2])-(len(x)-1)
    elif cart == 'DC':
        x = DC[1][random(len(DC[1]))]
        y = eval(DC[2])-(len(x)-1)
    elif cart == 'M':
        x = M[1][random(len(M[1]))]
        y = eval(M[2][random(len(M[2]))])-(len(x)-1)
    elif cart == 'MC':
        x = MC[1][random(len(MC[1]))]
        y = eval(MC[2])-(len(x)-1)
    elif cart == 'VE':
        x = VE[1][random(len(VE[1]))]
        y = eval(VE[2])-(len(x)-1)
    elif cart == 'V':
        x = V[1][random(len(V[1]))]
        y = eval(V[2][random(len(V[2]))])-(len(x)-1)
    yey= pre(x,y)
    fim = calc_soma(yey)
    re = eval(yey)*10 + ultimo_dig(fim)
    return re

def ultimo_dig(f):
    if f %10 ==0:
        return 0
    else: return 10 - f%10
    
def pre(x,y):
    dig=eval(x)
    while y > 0:
        dig = dig*10 + random(9)
        y -= 1
    return str(dig)

def random(t):
    import random
    return(int(random.random()*t))