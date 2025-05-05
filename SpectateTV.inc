// Simples sistema de espectador 24/05/2025 by Galo
/*
    v0.0.1 03/04/2025
        



*/
/*
Funçães
    StartSpectate(playerid, target);
    StopSpectate(playerid);
    GetTargetSpectatePlayer(playerid);

CallBacks
   public OnPlayerStartSpectate(playerid, target);
   public OnPlayerStopSpectate(playerid);
   public UpdateSpectate(playerid, target);

*/

forward OnPlayerStartSpectate(playerid, TargetID);
forward OnPlayerStopSpectate(playerid, TargetID);
forward UpdateSpectate(playerid, TargetID);

new IDTempSpecSys[MAX_PLAYERS] = 0;
new IDSpectateTarget[MAX_PLAYERS] = -1;
new oldPlayerVirtualWorld[MAX_PLAYERS] = 0;

public OnPlayerDisconnect(playerid, reason)
{
    StopSpectate(playerid);

    #if defined SysTV_OnPlayerDisconnect
        SysTV_OnPlayerDisconnect(playerid, reason);
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
	foreach(new x : Player)
    {
        if(IDSpectateTarget[x] == playerid)
            SpectateNow(x, playerid);
    }
    
    #if defined SysTV_OnPlayerSpawn
        SysTV_OnPlayerSpawn(playerid);
    #endif

	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) 
{
    foreach(new x : Player)
    {
        if(IDSpectateTarget[x] == playerid)
        {
            new Float:POSx, Float:POSy, Float:POSz;
            GetPlayerPos(playerid, POSx, POSy, POSz);

            new Float:cx, Float:cy, Float:cz;
            GetPlayerCameraPos(playerid, cx, cy, cz);

            cz += !GetPlayerInterior(playerid) ? 5.0 : 0.5;

            SetPlayerCameraPos(x, cx, cy, cz);
			SetPlayerCameraLookAt(x, POSx, POSy, POSz);
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

    if((IDPlayerState == PLAYER_STATE_ONFOOT) || (IDPlayerState == PLAYER_STATE_DRIVER) || (IDPlayerState == PLAYER_STATE_PASSENGER))
    {
        if(!IsPlayerInAnyVehicle(IDTarget))
        {
            SetPlayerInterior(playerid,GetPlayerInterior(IDTarget));
            SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(IDTarget));
            PlayerSpectatePlayer(playerid, IDTarget, 1);
        }
        else
        {
            SetPlayerInterior(playerid,GetPlayerInterior(IDTarget));
            SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(IDTarget));
            PlayerSpectateVehicle(playerid,GetPlayerVehicleID(IDTarget), 1);
        }
    }
    return 1;
}

// Começar assistir um jogador
StartSpectate(playerid, IDTarget)
{
    if(!IsPlayerConnected(playerid))
        return 0;
    if(!IsPlayerConnected(IDTarget))
        return 0;

    if(IDSpectateTarget[playerid] != -1)
    {
        CallLocalFunction("OnPlayerStopSpectate", "ii", playerid,IDSpectateTarget[playerid]);     
        IDSpectateTarget[playerid] = -1;
        KillTimer(IDTempSpecSys[playerid]);  
    }
    else
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
    if(IDSpectateTarget[playerid] != -1)
        CallLocalFunction("OnPlayerStopSpectate", "ii", playerid,IDSpectateTarget[playerid]);
    
    IDSpectateTarget[playerid] = -1;
    TogglePlayerSpectating(playerid, 0);
    KillTimer(IDTempSpecSys[playerid]);    
    return 1;
}

// Pegar o Id que o Player está Spectator
GetTargetSpectatePlayer(playerid) { return IDSpectateTarget[playerid]; }


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
