// Simples sistema de espectador dinamico by Galo v0.1.1
/*
Funçães
    StartSpectate(playerid, target);
    StopSpectate(playerid);
    GetTargetSpectatePlayer(playerid);
    IsPlayerModeSpectateTV(playerid);

CallBacks
   public OnPlayerStartSpectate(playerid, target);
   public OnPlayerStopSpectate(playerid);
   public UpdateSpectate(playerid, target);

*/

#if !defined _FOREACH_LOCAL_VERSION
    #error "Please include foreach"
#endif

forward OnPlayerStartSpectate(playerid, TargetID);
forward OnPlayerStopSpectate(playerid, TargetID);
forward UpdateSpectate(playerid, TargetID);

new IDTempSpecSys[MAX_PLAYERS], IDSpectateTarget[MAX_PLAYERS], oldPlayerVirtualWorld[MAX_PLAYERS];

public OnPlayerDisconnect(playerid, reason)
{
    StopSpectate(playerid);

    #if defined SysTV_OnPlayerDisconnect
        SysTV_OnPlayerDisconnect(playerid, reason);
    #endif
    return 1;
}

public OnPlayerConnect(playerid)
{
    IDTempSpecSys[playerid]             = -1;
    IDSpectateTarget[playerid]          = -1;
    oldPlayerVirtualWorld[playerid]     = -1;

    #if defined SysTV_OnPlayerConnect
        SysTV_OnPlayerConnect(playerid);
    #endif
    return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate) 
{
	if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER) 
    {
	    foreach(new x : Player)
        {
            if(IDSpectateTarget[x] == playerid)
            {
                PlayerSpectateVehicle(x,GetPlayerVehicleID(playerid), 1);
            }
        }
	}
	else if (newstate == PLAYER_STATE_ONFOOT) 
    {
        foreach(new x : Player)
        {
            if(IDSpectateTarget[x] == playerid)
                 PlayerSpectatePlayer(x, playerid, 1);
        }
	}
	else if (newstate == PLAYER_STATE_SPECTATING) 
    {
        foreach(new x : Player)
        {
            if(IDSpectateTarget[x] == playerid)
                 StopSpectate(x);
        }
	}

    #if defined SysTV_OnPlayerStateChange
        SysTV_OnPlayerStateChange(playerid, newstate, oldstate);
    #endif
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid) {
	foreach(new x : Player)
    {
        if(IDSpectateTarget[x] == playerid)
            SpectateNow(x, playerid);
    }
    #if defined SysTV_OnPlayerInteriorChange
        SysTV_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
    #endif
 	return 1;
}

public OnPlayerSpawn(playerid) 
{
    if(IsPlayerModeSpectateTV(playerid))
    {
        TogglePlayerSpectating(playerid, 1);
        SpectateNow(playerid, GetTargetSpectatePlayer(playerid));
    }    
    else
    {
        foreach(new x : Player)
        {
            if(IDSpectateTarget[x] == playerid)
                SpectateNow(x, playerid);
        }
    }

    #if defined SysTV_OnPlayerSpawn
        SysTV_OnPlayerSpawn(playerid);
    #endif

	return 1;
}

forward CamKillSpectate(playerid, TargetID);
public CamKillSpectate(playerid, TargetID)
{
    new Float:POSx, Float:POSy, Float:POSz;
    GetPlayerPos(playerid, POSx, POSy, POSz);

    new Float:cx, Float:cy, Float:cz;
    GetPlayerCameraPos(playerid, cx, cy, cz);

    cz += !GetPlayerInterior(playerid) ? 5.0 : 0.5;

    SetPlayerCameraPos(TargetID, cx, cy, cz);
    SetPlayerCameraLookAt(TargetID, POSx, POSy, POSz);
    return 1;
}

public OnPlayerDeath(playerid, killerid, reason) 
{
    if(!IsPlayerModeSpectateTV(playerid))
    {
        foreach(new x : Player)
        {
            if(IDSpectateTarget[x] == playerid)
            {
                TogglePlayerSpectating(x, 1);
                SetTimerEx("CamKillSpectate", 300, false, "ii", playerid, x);
                
            }
        }
    }

    
    #if defined SysTV_OnPlayerDeath
        SysTV_OnPlayerDeath(playerid, killerid, reason);
    #endif
	return 1;
}

forward UpdateSpecSys(playerid, IDTarget);
public UpdateSpecSys(playerid, IDTarget)
{
    new worldid = GetPlayerVirtualWorld(IDTarget);
	if (oldPlayerVirtualWorld[IDTarget] != worldid)
    {
        oldPlayerVirtualWorld[IDTarget] = worldid;
        SpectateNow(playerid, IDTarget);
	}

    CallLocalFunction("UpdateSpectate", "ii", playerid, IDTarget); 

    return 1;
}

SpectateNow(playerid, IDTarget)
{
    
    new IDPlayerState = GetPlayerState(IDTarget);
    oldPlayerVirtualWorld[IDTarget] = GetPlayerVirtualWorld(IDTarget);

    if((IDPlayerState == PLAYER_STATE_ONFOOT) || (IDPlayerState == PLAYER_STATE_DRIVER) || (IDPlayerState == PLAYER_STATE_PASSENGER))
    {
        if(!IsPlayerInAnyVehicle(IDTarget))
        {
            SetPlayerInterior(playerid,GetPlayerInterior(IDTarget));
            SetPlayerVirtualWorld(playerid,oldPlayerVirtualWorld[IDTarget]);
            PlayerSpectatePlayer(playerid, IDTarget, 1);
        }
        else
        {
            SetPlayerInterior(playerid,GetPlayerInterior(IDTarget));
            SetPlayerVirtualWorld(playerid,oldPlayerVirtualWorld[IDTarget]);
            PlayerSpectateVehicle(playerid,GetPlayerVehicleID(IDTarget), 1);
        }
    }
    return 1;
}

// Começar assistir um jogador
StartSpectate(playerid, IDTarget)
{
    if(!IsPlayerConnected(IDTarget) || IsPlayerModeSpectateTV(IDTarget) || !IsPlayerConnected(playerid) || playerid == IDTarget)
        return 0;
        
    // Finalizar outro Spectate
    if(IDSpectateTarget[playerid] != -1)
    {
        CallLocalFunction("OnPlayerStopSpectate", "ii", playerid,IDSpectateTarget[playerid]);     
        IDSpectateTarget[playerid] = -1;
    }
    
    KillTimer(IDTempSpecSys[playerid]);  
    TogglePlayerSpectating(playerid, 1);
    
    IDSpectateTarget[playerid] = IDTarget;
    SpectateNow(playerid, IDTarget);
    IDTempSpecSys[playerid] = SetTimerEx("UpdateSpecSys", 1000, true, "ii", playerid, IDTarget);
    CallLocalFunction("OnPlayerStartSpectate", "ii", playerid, IDTarget); 
    return 1;
}

// Para de assistir um jogador
StopSpectate(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(IDSpectateTarget[playerid] != -1)
        CallLocalFunction("OnPlayerStopSpectate", "ii", playerid,IDSpectateTarget[playerid]);
    
    IDSpectateTarget[playerid] = -1;
    TogglePlayerSpectating(playerid, 0);
    KillTimer(IDTempSpecSys[playerid]);    
    return 1;
}

// Pegar o Id que o jogador está assistindo
GetTargetSpectatePlayer(playerid) 
{ 
    if(!IsPlayerConnected(playerid))
        return -1;
        
    return IDSpectateTarget[playerid]; 
}

// Verificar se o Jogador está no modo spectator dinamico
IsPlayerModeSpectateTV(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(IDSpectateTarget[playerid] != -1)
        return 1;
    else
        return 0;
}

// Method ALS OnPlayerDisconnect
    #if defined _ALS_OnPlayerDisconnect
        #undef OnPlayerDisconnect
    #else
        #define _ALS_OnPlayerDisconnect
    #endif

    #define OnPlayerDisconnect SysTV_OnPlayerDisconnect

    #if defined SysTV_OnPlayerDisconnect
        forward OnPlayerDisconnect(playerid, reason);
    #endif

// Method ALS OnPlayerConnect
    #if defined _ALS_OnPlayerConnect
        #undef OnPlayerConnect
    #else
        #define _ALS_OnPlayerConnect
    #endif

    #define OnPlayerConnect SysTV_OnPlayerConnect

    #if defined SysTV_OnPlayerConnect
        forward OnPlayerConnect(playerid);
    #endif

// Method ALS OnPlayerStateChange
    #if defined _ALS_OnPlayerStateChange
        #undef OnPlayerStateChange
    #else
        #define _ALS_OnPlayerStateChange
    #endif

    #define OnPlayerStateChange SysTV_OnPlayerStateChange

    #if defined SysTV_OnPlayerStateChange
        forward OnPlayerStateChange(playerid, newstate, oldstate);
    #endif

// Method ALS OnPlayerInteriorChange
    #if defined _ALS_OnPlayerInteriorChange
        #undef OnPlayerInteriorChange
    #else
        #define _ALS_OnPlayerInteriorChange
    #endif

    #define OnPlayerInteriorChange SysTV_OnPlayerInteriorChange

    #if defined SysTV_OnPlayerInteriorChange
        forward OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
    #endif
// Method ALS OnPlayerSpawn
    #if defined _ALS_OnPlayerSpawn
        #undef OnPlayerSpawn
    #else
        #define _ALS_OnPlayerSpawn
    #endif

    #define OnPlayerSpawn SysTV_OnPlayerSpawn

    #if defined SysTV_OnPlayerSpawn
        forward OnPlayerSpawn(playerid);
    #endif
// Method ALS OnPlayerDeath
    #if defined _ALS_OnPlayerDeath
        #undef OnPlayerDeath
    #else
        #define _ALS_OnPlayerDeath
    #endif

    #define OnPlayerDeath SysTV_OnPlayerDeath

    #if defined SysTV_OnPlayerDeath
        forward OnPlayerDeath(playerid, killerid, reason);
    #endif