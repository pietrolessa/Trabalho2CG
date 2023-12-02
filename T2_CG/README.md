# Robô Articulado Atirador

## Objetivo
Criar um jogo, em 3D, no qual o usuárioo controla um canhão capaz de atirar. O jogador deve destruir inimigos que estão posicionados atrás de um paredão.

Ao atingir o paredão, uma parte dele deve ser destruída, e o projétil também.

Além dos inimigos, também existem objetos amigos, que não devem ser destruídos.

## Lista de Checks

<ul><li>✅ Criar Piso</li></ul> 

    - 50x25. Feito com quadrados de 1x1
    - Textura semelhante a grama
        - Imagem única, cada pedaço recebe uma parte da imagem

<ul><li>✅ Criar Paredão</li></ul>

    - 15 de altura, no meio do cenário
    - Textura de tijolos
        - Imagem única, cada pedaço recebe uma parte da imagem

<ul><li>✅ Criar Canhão</li></ul>

    - 3x2, formado por um paraleleípedo
    - deve ter 2 atiradores
    - velocidade = 2.5m/s
    - possivel aumentar e diminuir força de lançamento de projétil

<ul><li>✅ Criar Braços Articulados</li></ul>

    - centro da frente do canhao, vamos fazer com 1 de altura
    Q - gira o primeiro braco em sentido horário
    E - gira segundo braco em sentido horario


<ul><li>✅ Criar Projétil</li></ul>

    - esfera do tamanho do canhão
    - movimento deve seguir formula física de lançamento de projéteis
  
    - ao atingir um objeto, ganha ou perde pontos:
      
          - Objeto inimigo: +10 pontos
          - Objeto amigo: -10 pontos
          - Piso: -5 pontos
          - Paredão: +5 pontos
          - Canhão: fim do jogo

<ul><li>⬜ Colisão no paredão</li></ul>

    - Deve parar de renderizar a parte (8 poligonos ao redor do atingido)
    - Não deve gerar colisão com o projétil ou o veículo

<ul><li>⬜ Inimigos e Amigos</li></ul>

    - pelo menos 20 objetos 3D lidos de arquivo no formato TRI
    - 50% inimigos 50% amigos

<ul><li>⬜ Lançamento do projetil</li></ul>

    - Deve parar de renderizar a parte (8 poligonos ao redor do atingido)
    - Não deve gerar colisão com o projétil ou o veículo


### Mais informações em <li>https://www.inf.pucrs.br/pinho/CG/Trabalhos/T2-2023-2-TiroNoParedao/T3-2023-2.html</li>

## Tracking notas

| **Feito?** |**Critérios** | **Pesos** | **Observações** |
|------------|--------------|-----------|-----------------|
|✅| Desenho do Paredão | 2,0 | O desenho deve ter feito com cada célula sendo gerada individualmente, com uma textura aplicada a cada célula. A textura deve vir da mesma imagem, mas em cada célula, uma parte diferente da imagem deve ser mapeada.
| | Detecção da colisão do projétil com o paredão e com os objetos do cenário | 1,5 | Este item só será considerado 100% correto se for possível mover o veículo corretamente e fazer os disparos conforme a especificação
| | Reconfiguração do Paredão após a colisão com o projétil | 1,0 | O paredão deve apresentar buracos nos pontos em que os projéteis atingirem. Devem ser destruídos os 8 (oito) polígonos ao redor do polígono que foi atingido.
|✅| Modelagem do Veículo com as articulações móveis | 1,5 | 
|✅| Movimentação do Veículo - girar e andar para frente | 1,0 | 
| | Lançamento do Projétil, com base no ângulo do canhão e do veículo | 1,5 | A trajetória deve ser definida de acordo com a direção do veículo, a inclinação do canhão e a força definida para o disparo.
| | Exibição correta dos objetos no formato TRI | 1,5 | A exibição deve levar em conta o cálculo dos vetores normais das faces.

