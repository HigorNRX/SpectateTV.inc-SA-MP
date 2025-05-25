#include <a_samp>
#include <core>
#include <float>
#include <string>
#include <foreach>
#include <zcmd>
#include <sscanf2>


#include "projects\SpectateTV.inc"
#pragma tabsize 0

main()
{
	print("\n----------------------------------");
	print("  SpectateTV_Test\n");
	print("----------------------------------\n");
}

public OnPlayerConnect(playerid)
{
	GameTextForPlayer(playerid,"~w~SA-MP: ~r~SpectateTV_Test Script",5000,5);
	return 1;
}

CreateSimpleVehicle(playerid, modelid, Float:x, Float:y, Float:z, Float:angle)
{
    new vehicleid = CreateVehicle(modelid, x, y, z, angle, -1, -1, 600);
    PutPlayerInVehicle(playerid, vehicleid, 0);
    return vehicleid;
}

public OnPlayerCommandText(playerid, cmdtext[])
{

    return 0;
}

public OnPlayerSpawn(playerid)
{
	//ShowDelimitedAreaForPlayer(playerid, idmapdel);
	SetPlayerInterior(playerid,0);
	TogglePlayerClock(playerid,0);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
   	return 0;
}

SetupPlayerForClassSelection(playerid)
{
 	SetPlayerInterior(playerid,14);
	SetPlayerPos(playerid,258.4893,-41.4008,1002.0234);
	SetPlayerFacingAngle(playerid, 270.0);
	SetPlayerCameraPos(playerid,256.0815,-43.0475,1004.0234);
	SetPlayerCameraLookAt(playerid,258.4893,-41.4008,1002.0234);
}

public OnPlayerRequestClass(playerid, classid)
{
	SetupPlayerForClassSelection(playerid);
	return 1;
}

public OnGameModeInit()
{
	SetGameModeText("SpectateTV_Test");
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	//idmapdel = CreateDelimitArea(1995.9215,1395.4008,2086.9895,1297.2981, 0, false);
	AddPlayerClass(265,1958.3783,1343.1572,15.3746,270.1425,0,0,0,0,-1,-1);
	UsePlayerPedAnims();
	printf("%i", IDSpectateTarget[1]);
	return 1;
}


public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    // Verifica se o jogador está logado ou em estado válido, se necessário
    SetPlayerPos(playerid, fX, fY, fZ + 2.0); // Adiciona 2.0 em Z para evitar cair no chão
    SendClientMessage(playerid, -1, "Você foi teleportado para o local clicado no mapa.");
    return 1;
}

CMD:tv(playerid, params[])
{
	new IdTvP;
	sscanf(params, "i", IdTvP);

	StartSpectate(playerid, IdTvP);

	return 1;
}

CMD:mudarvw(playerid, params[])
{
    new vw;
    if(sscanf(params, "d", vw)) return SendClientMessage(playerid, -1, "Uso: /mudarvw [id do virtual world]");

    SetPlayerVirtualWorld(playerid, vw);
    SendClientMessage(playerid, -1, "Você mudou de Virtual World com sucesso!");
    return 1;
}


CMD:stoptv(playerid)
{
    StopSpectate(playerid);
    return 1;
}
CMD:car(playerid)
{
    new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);
	CreateSimpleVehicle(playerid, 411, x + 3.0, y, z, a);
    return 1;
}

CMD:killme(playerid)
{
    SetPlayerHealth(playerid, 0.0);
    return 1;
}
CMD:s1(playerid)
{
    TogglePlayerSpectating(playerid, 1);
    return 1;
}
CMD:s0(playerid)
{
    TogglePlayerSpectating(playerid, 0);
    return 1;
}