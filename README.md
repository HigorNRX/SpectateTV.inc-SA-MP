# 📺 Simples Sistema de Espectador

Um sistema básico de espectador para SA:MP, permitindo que um jogador observe outro jogador facilmente.

> 🛠 Desenvolvido por **Galo**  
> 📅 Iniciado em **24/05/2025**  
> 📦 Versão: `v0.0.1` (03/04/2025)

---

## 📌 Funcionalidades

- Início e parada de espectador com controle por player.
- Retorno do alvo atual sendo observado.
- Callbacks personalizáveis para integração com o seu sistema.

---

## ⚙️ Funções

```pawn
StartSpectate(playerid, target);         // Inicia o modo espectador no 'target'
StopSpectate(playerid);                  // Encerra o modo espectador
GetTargetSpectatePlayer(playerid);       // Retorna o ID do jogador sendo observado

## ⚙️ CallBacks
public OnPlayerStartSpectate(playerid, target); // Chamado ao iniciar espectador
public OnPlayerStopSpectate(playerid);          // Chamado ao parar espectador
public UpdateSpectate(playerid, target);        // Chamado para atualizar estado do espectador
