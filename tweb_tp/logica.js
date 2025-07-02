function ShowDropMenu(n){
    var element = document.getElementsByClassName('dropdown-menu'); 
    for (var i = 0; i < element.length; i++){
        if(element[i].classList.contains("show")){
            element[i].classList.remove("show");
        }
    }
    element[n].classList.toggle("show");
}

//Event Listeners
window.onclick = function(event){
    var element = document.getElementsByClassName('dropdown-menu'); 
    var eleOpened;
    for (var i = 0; i < element.length; i++){
        if(element[i].classList.contains("show")){
            eleOpened = true
        }
        else eleOpened = false;
    }
    if (!event.target.matches(".dropdown-toggle") && !event.target.closest(".dropdown-menu"))
        for (var i = 0; i < element.length; i++){
            if(element[i].classList.contains("show")){
                element[i].classList.remove("show");
            }
        }
    // if (eleOpened)
    // {
    //     if (!event.target.closest(".dropdown-menu"))
    //     for (var i = 0; i < element.length; i++){
    //         if(element[i].classList.contains("show")){
    //             element[i].classList.remove("show");
    //         }
    //     }
    // }
    // else{
    //     if (!event.target.matches(".dropdown-toggle") && !event.target.closest(".dropdown-menu"))
    //     for (var i = 0; i < element.length; i++){
    //         if(element[i].classList.contains("show")){
    //             element[i].classList.remove("show");
    //         }
    //     }
    // }
    
}

// function ShowDropMenu(n){
//     var element = document.getElementsByClassName('dropdown-menu'); 
//    element[n].classList.toggle("show");
// }

//Mobile menu
function ToggleMobileMenu(){
    document.getElementsByClassName("mobile-menu")[0].classList.toggle("show");
    document.getElementsByClassName("menu-button-mobile")[0].classList.toggle("open");
}


//Checkbox like radiobuton
function ClearCheck(n){
    var checkbox = document.getElementsByName("group1[]");
    for (var i = 0; i < checkbox.length; i++){
        checkbox[i].checked = false;
    }
    checkbox[n].checked = true;
}

function ClearCheck2(n){
    var checkbox = document.getElementsByName("group2[]");
    for (var i = 0; i < checkbox.length; i++){
        checkbox[i].checked = false;
    }
    checkbox[n].checked = true;
}

//Start
document.getElementsByName("area")[0].value = "25";
document.getElementsByName("group1[]")[0].checked = true;
document.getElementsByName("group2[]")[0].checked = true;
document.getElementsByName("prox")[0].checked = true;
document.getElementsByName("garagem")[0].checked = true;



 // prazo para pagar o emprestimo - anos INTRODUDIZO PELO UTILIZADOR
    // montante total (valor da casa) - valor da casa INTRODUDIZO PELO UTILIZADOR
    // valor do empréstimo - valor total do emprestimo INTRODUDIZO PELO UTILIZADOR
    // taxa de juro global - spread + 0.5%
    // spread - random entre 1 e 5%
    // valor da entrada inicial - valor de entrada do emprestimo INTRODUDIZO PELO UTILIZADOR
    // valor mensal da prestação - valor do emprestimo * taxa de juro global

    var precofinal1 = 0;
    var precofinal2 = 0;
    var precofinal3 = 0;
    
    function calc(){
        var buttondisable = document.getElementById("btncalc");
        buttondisable.disabled = true;

        var elementox = document.getElementById("resultados");
        elementox.style.display = "block";

        var elementoy = document.getElementById("anunciosmain");
        elementoy.style.paddingTop = "50px";

        var tipologia = document.getElementsByName("group1[]"); //T0, T1, T2, T3, T4, T5, T5+
        for (var i = 0; i < tipologia.length; i++){
            if(tipologia[i].checked){
                tipologia = tipologia[i].value;
            }
        }
        var area = document.getElementById("area").value; //number
        var idade = document.getElementsByName("group2[]"); //opc1, opc2, opc3
        for (var i = 0; i < idade.length; i++){
            if(idade[i].checked){
                idade = idade[i].value;
            }
        }
        var garagem = document.getElementById("garagem_sim").checked; //true or false
        var transportespublicos = document.getElementById("transportespublicos_sim").checked; //true or false
        //CALCULO DE FATORES DE DESVALORIZAÇÃO
        var idadef = "0<i<=5", garagemf = "Sim", transportespublicosf = "Sim";
    
        var desvalorizacao = 0;
    
        if(idade == "opc2"){
            idadef = "5<i<=10";
            desvalorizacao += 0.05;
        }else if(idade == "opc3"){
            idadef = "i>10";
            desvalorizacao += 0.1;
        }
    
        if(garagem == false){
            garagemf = "Não;"
            desvalorizacao += 0.05;
        }
    
        if(transportespublicos == false){
            transportespublicosf = "Não"
            desvalorizacao += 0.1;
        }
    
        //CALCULO ZONAS= 1200 / 2000 / 2500 INCLUINDO A DESVALORIZAÇÃO
    
        var precozona1 = (area * 1200)*(1-desvalorizacao);
        var precozona2 = (area * 2000)*(1-desvalorizacao);
        var precozona3 = (area * 2500)*(1-desvalorizacao);
    
    
    
        //mostrar preços para 3 zonas diferentes
    
        if(isNaN(area)){ //verifica se is not a number, return true or false
            alert("Área tem de ser um número");
        }else{
            document.getElementById('texta1').innerHTML += "Zona1: \n\nPreço m2: 1200\nÁrea: "+area+"\nTipologia: "+tipologia+"\nIdade: "+idadef+"\nGaragem: "+garagemf+"\nTransportes públicos: "+transportespublicosf+"\nTaxa de desvalorização: "+desvalorizacao.toFixed(2)+" %\nPREÇO TOTAL: "+precozona1+" €";
            document.getElementById('texta2').innerHTML += "Zona2: \n\nPreço m2: 2000\nÁrea: "+area+"\nTipologia: "+tipologia+"\nIdade: "+idadef+"\nGaragem: "+garagemf+"\nTransportes públicos: "+transportespublicosf+"\nTaxa de desvalorização: "+desvalorizacao.toFixed(2)+" %\nPREÇO TOTAL: "+precozona2+" €";
            document.getElementById('texta3').innerHTML += "Zona3: \n\nPreço m2: 2500\nÁrea: "+area+"\nTipologia: "+tipologia+"\nIdade: "+idadef+"\nGaragem: "+garagemf+"\nTransportes públicos: "+transportespublicosf+"\nTaxa de desvalorização: "+desvalorizacao.toFixed(2)+" %\nPREÇO TOTAL: "+precozona3+" €";
        }
    
        precofinal1 = precozona1;
        precofinal2 = precozona2;
        precofinal3 = precozona3;
    
        // TOTAL * TAXA DE JURO GLOBAL / ANOS / 12
        // VALOR EXPRESTIMO = TOTAL - (TOTAL * TAXA DE JUROBLOBAL)
    
    }

    function showfin(){
        var elementox = document.getElementById("financiamentotab");
        elementox.style.display = "block";
    }
    
    function finan(){
    
        //var totalmontanteinicial = document.getElementById("totalfinanciamento").value;
    
        var preco1 = precofinal1;
        var preco2 = precofinal2;
        var preco3 = precofinal3;

        console.log(preco1);
        console.log(preco2);
        console.log(preco3);
    
        var entrada = document.getElementById("entradainicial").value;
        var anos = document.getElementById("anoss").value;

        if(entrada >= preco3){
            alert("Valor de entrada não pode ser superior ao preço total das propriedades");
        }else{
            var buttondisable = document.getElementById("btnfinan");
            buttondisable.disabled = true;

            var elementox = document.getElementById("resultadosfinan");
            elementox.style.display = "block";

            var totalmontante1 = preco1 - entrada;
            var totalmontante2 = preco2 - entrada;
            var totalmontante3 = preco3 - entrada;
        
            //RANDOM SPREADS ENTRE 1 E 5
        
            var spread1 = ((Math.random() * 5) + 1).toFixed(2)
            var spread2 = ((Math.random() * 5) + 1).toFixed(2)
            var spread3 = ((Math.random() * 5) + 1).toFixed(2)
        
            var juros1 = spread1;
            var juros2 = spread2;
            var juros3 = spread3;

            console.log("total: "+totalmontante1 + "juros: "+juros1);
        
            var valordoemprestimo1 = (totalmontante1 - (totalmontante1*juros1/100)).toFixed(2)
            var valordoemprestimo2 = (totalmontante2 - (totalmontante2*juros2/100)).toFixed(2)
            var valordoemprestimo3 = (totalmontante3 - (totalmontante3*juros3/100)).toFixed(2)

            console.log(valordoemprestimo1);
        
            var meses1 = (valordoemprestimo1 / anos / 12).toFixed(2);
            var meses2 = (valordoemprestimo2 / anos / 12).toFixed(2);
            var meses3 = (valordoemprestimo3 / anos / 12).toFixed(2);
        
        
            document.getElementById('textb1').innerHTML += "Valor total do imovel: "+preco1+" €\nEntrada Inicial: "+entrada+" €\nValor em falta: "+totalmontante1+" €\nValor do Emprestimo: "+valordoemprestimo1+" €\nTaxa de juro global: "+juros1+" %\nPrestação mensal: "+meses1+" €";
            document.getElementById('textb2').innerHTML += "Valor total do imovel: "+preco2+" €\nEntrada Inicial: "+entrada+" €\nValor em falta: "+totalmontante2+" €\nValor do Emprestimo: "+valordoemprestimo2+" €\nTaxa de juro global: "+juros2+" %\nPrestação mensal: "+meses2+" €";
            document.getElementById('textb3').innerHTML += "Valor total do imovel: "+preco3+" €\nEntrada Inicial: "+entrada+" €\nValor em falta: "+totalmontante3+" €\nValor do Emprestimo: "+valordoemprestimo3+" €\nTaxa de juro global: "+juros3+" %\nPrestação mensal: "+meses3+" €";
        }

        console.log(entrada);
        console.log(anos);
    
        
    }

    function clearr(){
        location.reload();
    }