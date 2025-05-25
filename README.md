# 📺 Simples Sistema de Espectador

Um sistema básico de espectador para SA:MP, permitindo que um jogador observe outro jogador sem interrupções.

> 🛠 Desenvolvido por **HigorNRX - Galo**  
> 📅 Iniciado em **03/04/2025**  
> 📦 Versão: `v0.1.1` (24/05/2025)

---

## 📌 Funcionalidades

- Início e parada de espectador.
- Callbacks para integração com o seu sistema.
- Camera de morte

---

## ⚙️ Funções
```pawn
StartSpectate(playerid, target);         // Inicia o modo espectador no 'target'
StopSpectate(playerid);                  // Encerra o modo espectador
GetTargetSpectatePlayer(playerid);       // Retorna o ID do jogador sendo observado
IsPlayerModeSpectateTV(playerid);        // Verificar se o jogador está no modo espectador
```
## ⚙️ CallBacks
```pawn
public OnPlayerStartSpectate(playerid, target); // Chamado ao iniciar espectador
public OnPlayerStopSpectate(playerid);          // Chamado ao parar espectador
public UpdateSpectate(playerid, target);        // Chamado para atualizar a cena
