// WaterPhysics Plugin for Union SDK
// Adds water splash effects for arrows, dropped items and weapon attacks
// Effects scale based on object size
#include "resource.h"

namespace GOTHIC_ENGINE {
  
  enum WaterEffectSize {
    EFFECT_SIZE_TINY,
    EFFECT_SIZE_SMALL,
    EFFECT_SIZE_MEDIUM,
    EFFECT_SIZE_LARGE
  };
  
  static const float SIZE_THRESHOLD_TINY = 500.0f;
  static const float SIZE_THRESHOLD_SMALL = 2000.0f;
  static const float SIZE_THRESHOLD_MEDIUM = 8000.0f;
  
  static bool configItemEffects = true;
  static bool configArrowEffects = true;
  static bool configWeaponEffects = true;
  
  void LoadConfiguration() {
    zoptions->ChangeDir(DIR_ROOT);
    
    configItemEffects = zoptions->ReadBool("WATER_PHYSICS", "ItemEffects", true);
    configArrowEffects = zoptions->ReadBool("WATER_PHYSICS", "ArrowEffects", true);
    configWeaponEffects = zoptions->ReadBool("WATER_PHYSICS", "WeaponEffects", true);
  }
  
  static zCArray<zCVob*> splashedVobs;
  
  struct VobTrackData {
    zCVob* vob;
    zVEC3 lastPos;
    bool wasAboveWater;
  };
  static zCArray<VobTrackData> trackedVobs;
  
  struct DroppedItemData {
    zCVob* vob;
    float dropTime;
    float waterLevel;
    bool splashed;
    WaterEffectSize effectSize;
  };
  static zCArray<DroppedItemData> droppedItems;
  
  static float lastWeaponSplashTime = 0.0f;
  static const float WEAPON_SPLASH_COOLDOWN = 300.0f;
  static const float DROPPED_ITEM_TRACK_TIME = 5000.0f;

  bool IsWorldReady() {
    if (!ogame) return false;
    if (ogame->inScriptStartup || ogame->inLoadSaveGame || ogame->inLevelChange) return false;
    return true;
  }

  bool StringContains(const zSTRING& str, const char* substr) {
    if (!substr) return false;
    const char* strData = str.ToChar();
    if (!strData) return false;
    return strstr(strData, substr) != nullptr;
  }

  int FindTrackedVob(zCVob* vob) {
    for (int i = 0; i < trackedVobs.GetNum(); i++) {
      if (trackedVobs[i].vob == vob) return i;
    }
    return -1;
  }

  bool HasSplashed(zCVob* vob) {
    for (int i = 0; i < splashedVobs.GetNum(); i++) {
      if (splashedVobs[i] == vob) return true;
    }
    return false;
  }

  float GetWaterLevelAt(const zVEC3& position) {
    if (!ogame || !ogame->GetGameWorld()) return -99999.0f;
    
    zCWorld* world = ogame->GetGameWorld();
    zVEC3 rayStart = position + zVEC3(0, 500, 0);
    zVEC3 rayDir = zVEC3(0, -1000, 0);
    int flags = zTRACERAY_STAT_POLY | zTRACERAY_POLY_TEST_WATER;
    
    if (world->TraceRayFirstHit(rayStart, rayDir, (zCVob*)nullptr, flags)) {
      zCPolygon* poly = world->traceRayReport.foundPoly;
      if (poly && poly->material) {
        if (poly->material->matGroup == zMAT_GROUP_WATER) {
          return world->traceRayReport.foundIntersection[VY];
        }
        
        zSTRING matName = poly->material->GetName();
        matName.Upper();
        const char* matStr = matName.ToChar();
        if (matStr && (strstr(matStr, "WATER") != nullptr || strstr(matStr, "WASSER") != nullptr)) {
          return world->traceRayReport.foundIntersection[VY];
        }
      }
    }
    return -99999.0f;
  }

  bool CheckWaterEntry(zCVob* vob) {
    if (!vob || !IsWorldReady()) return false;
    
    zVEC3 currentPos = vob->GetPositionWorld();
    float waterLevel = GetWaterLevelAt(currentPos);
    if (waterLevel < -99000.0f) return false;
    
    int idx = FindTrackedVob(vob);
    if (idx < 0) {
      VobTrackData data;
      data.vob = vob;
      data.lastPos = currentPos;
      data.wasAboveWater = currentPos[VY] > waterLevel;
      trackedVobs.Insert(data);
      return false;
    }
    
    VobTrackData& data = trackedVobs[idx];
    bool isAboveWater = currentPos[VY] > waterLevel;
    bool enteredWater = data.wasAboveWater && !isAboveWater;
    data.lastPos = currentPos;
    data.wasAboveWater = isAboveWater;
    return enteredWater;
  }

  void RemoveTrackedVob(zCVob* vob) {
    int idx = FindTrackedVob(vob);
    if (idx >= 0) trackedVobs.RemoveIndex(idx);
    
    for (int i = 0; i < splashedVobs.GetNum(); i++) {
      if (splashedVobs[i] == vob) {
        splashedVobs.RemoveIndex(i);
        break;
      }
    }
  }

  int FindDroppedItem(zCVob* vob) {
    for (int i = 0; i < droppedItems.GetNum(); i++) {
      if (droppedItems[i].vob == vob) return i;
    }
    return -1;
  }

  WaterEffectSize CalculateEffectSize(zCVob* vob) {
    if (!vob) return EFFECT_SIZE_SMALL;
    
    zTBBox3D bbox = vob->GetBBox3DLocal();
    
    float sizeX = bbox.maxs[VX] - bbox.mins[VX];
    float sizeY = bbox.maxs[VY] - bbox.mins[VY];
    float sizeZ = bbox.maxs[VZ] - bbox.mins[VZ];
    
    if (sizeX < 0) sizeX = -sizeX;
    if (sizeY < 0) sizeY = -sizeY;
    if (sizeZ < 0) sizeZ = -sizeZ;
    
    float volume = sizeX * sizeY * sizeZ;
    
    float maxExtent = sizeX;
    if (sizeY > maxExtent) maxExtent = sizeY;
    if (sizeZ > maxExtent) maxExtent = sizeZ;
    
    float effectiveSize = volume;
    if (maxExtent > 100.0f && volume < 1000.0f) {
      effectiveSize = volume * 0.5f;
    }
    
    if (effectiveSize < SIZE_THRESHOLD_TINY) {
      return EFFECT_SIZE_TINY;
    } else if (effectiveSize < SIZE_THRESHOLD_SMALL) {
      return EFFECT_SIZE_SMALL;
    } else if (effectiveSize < SIZE_THRESHOLD_MEDIUM) {
      return EFFECT_SIZE_MEDIUM;
    } else {
      return EFFECT_SIZE_LARGE;
    }
  }
  
  const char* GetRingEffectName(WaterEffectSize size) {
    switch (size) {
      case EFFECT_SIZE_TINY:   return "spellFX_WaterRing_Tiny";
      case EFFECT_SIZE_SMALL:  return "spellFX_WaterRing_Small";
      case EFFECT_SIZE_MEDIUM: return "spellFX_WaterRing_Medium";
      case EFFECT_SIZE_LARGE:  return "spellFX_WaterRing_Large";
      default:                 return "spellFX_WaterRing_Small";
    }
  }
  
  // Get VFX name for splash effect based on size
  const char* GetSplashEffectName(WaterEffectSize size) {
    switch (size) {
      case EFFECT_SIZE_TINY:   return "spellFX_WaterSplash_Tiny";
      case EFFECT_SIZE_SMALL:  return "spellFX_WaterSplash_Small";
      case EFFECT_SIZE_MEDIUM: return "spellFX_WaterSplash_Medium";
      case EFFECT_SIZE_LARGE:  return "spellFX_WaterSplash_Large";
      default:                 return "spellFX_WaterSplash_Small";
    }
  }

  void CreateWaterSplash(const zVEC3& pos, WaterEffectSize effectSize) {
    if (!ogame || !ogame->GetWorld()) return;
    
    zVEC3 effectPos = pos;
    effectPos[VY] += 5.0f;
    
    const char* splashName = GetSplashEffectName(effectSize);
    oCVisualFX::CreateAndPlay(zSTRING(splashName), effectPos, nullptr, 0, 0.0f, 0, 0);
    
    const char* ringName = GetRingEffectName(effectSize);
    oCVisualFX::CreateAndPlay(zSTRING(ringName), effectPos, nullptr, 0, 0.0f, 0, 0);
    
    if (zsound) {
      zCVob* soundVob = new zCVob();
      soundVob->SetPositionWorld(effectPos);
      ogame->GetWorld()->AddVob(soundVob);
      
      zCSoundSystem::zTSound3DParams params;
      params.SetDefaults();
      params.volume = 1.0f;
      params.radius = 2000.0f;
      
      zsound->PlaySound3D(zSTRING("WATERPHYSICS_SPLASH"), soundVob, 0, &params);
      
      ogame->GetWorld()->RemoveVob(soundVob);
      soundVob->Release();
    }
  }
  
  void CreateArrowSplash(const zVEC3& pos) {
    CreateWaterSplash(pos, EFFECT_SIZE_TINY);
  }

  void TrackDroppedItem(zCVob* vob) {
    if (!vob || !IsWorldReady()) return;
    if (FindDroppedItem(vob) >= 0) return;
    
    zVEC3 pos = vob->GetPositionWorld();
    DroppedItemData data;
    data.vob = vob;
    data.dropTime = ztimer->totalTimeFloat;
    data.waterLevel = GetWaterLevelAt(pos);
    data.splashed = false;
    data.effectSize = CalculateEffectSize(vob);
    droppedItems.Insert(data);
  }

  void UpdateDroppedItems() {
    if (!IsWorldReady()) return;
    
    float currentTime = ztimer->totalTimeFloat;
    
    for (int i = droppedItems.GetNum() - 1; i >= 0; i--) {
      DroppedItemData& data = droppedItems[i];
      
      if (currentTime - data.dropTime > DROPPED_ITEM_TRACK_TIME) {
        droppedItems.RemoveIndex(i);
        continue;
      }
      
      if (data.splashed || !data.vob) {
        if (!data.vob) droppedItems.RemoveIndex(i);
        continue;
      }
      
      zVEC3 pos = data.vob->GetPositionWorld();
      
      if (data.waterLevel < -99000.0f) {
        data.waterLevel = GetWaterLevelAt(pos);
      }
      
      if (data.waterLevel > -99000.0f && pos[VY] <= data.waterLevel + 10.0f) {
        zVEC3 splashPos = pos;
        splashPos[VY] = data.waterLevel;
        CreateWaterSplash(splashPos, data.effectSize);
        data.splashed = true;
        splashedVobs.Insert(data.vob);
      }
    }
  }

  static void (__thiscall *oCAIArrow_DoAI_Org)(oCAIArrow*, zCVob*, int&) = nullptr;
  
  void __fastcall oCAIArrow_DoAI_Hook(oCAIArrow* _this, void*, zCVob* vob, int& deleteVob) {
    if (_this && vob && IsWorldReady() && configArrowEffects) {
      if (CheckWaterEntry(vob)) {
        zVEC3 pos = vob->GetPositionWorld();
        float wl = GetWaterLevelAt(pos);
        if (wl > -99000.0f) {
          pos[VY] = wl;
          CreateArrowSplash(pos);
          splashedVobs.Insert(vob);
        }
      }
    }
    
    if (oCAIArrow_DoAI_Org) {
      oCAIArrow_DoAI_Org(_this, vob, deleteVob);
    }
    
    if (deleteVob) RemoveTrackedVob(vob);
  }

  static void (__thiscall *oCAIArrow_ReportCollision_Org)(oCAIArrow*, zCCollisionReport const&) = nullptr;
  
  void __fastcall oCAIArrow_ReportCollision_Hook(oCAIArrow* _this, void*, zCCollisionReport const& report) {
    if (_this && _this->vob && IsWorldReady() && configArrowEffects) {
      zCVob* vob = _this->vob;
      zVEC3 pos = vob->GetPositionWorld();
      float waterLevel = GetWaterLevelAt(pos);
      
      if (waterLevel > -99000.0f) {
        float dist = pos[VY] - waterLevel;
        if (dist < 50.0f && dist > -100.0f && !HasSplashed(vob)) {
          pos[VY] = waterLevel;
          CreateArrowSplash(pos);
          splashedVobs.Insert(vob);
        }
      }
    }
    
    if (oCAIArrow_ReportCollision_Org) {
      oCAIArrow_ReportCollision_Org(_this, report);
    }
  }

  HOOK Hook_oCNpc_DoDropVob PATCH(&oCNpc::DoDropVob, &oCNpc::DoDropVob_Union);
  HOOK Hook_oCNpc_DoThrowVob PATCH(&oCNpc::DoThrowVob, &oCNpc::DoThrowVob_Union);

  int oCNpc::DoDropVob_Union(zCVob* vob) {
    int result = THISCALL(Hook_oCNpc_DoDropVob)(vob);
    if (vob && IsWorldReady() && configItemEffects) {
      TrackDroppedItem(vob);
    }
    return result;
  }

  int oCNpc::DoThrowVob_Union(zCVob* vob, float force) {
    int result = THISCALL(Hook_oCNpc_DoThrowVob)(vob, force);
    if (vob && IsWorldReady() && configItemEffects) {
      TrackDroppedItem(vob);
    }
    return result;
  }

  bool IsInMeleeAttackAni(oCNpc* npc) {
    if (!npc) return false;
    zCModel* model = npc->GetModel();
    if (!model) return false;
    
    for (int i = 0; i < model->numActiveAnis; i++) {
      zCModelAniActive* aniActive = model->aniChannels[i];
      if (!aniActive || !aniActive->protoAni) continue;
      
      zSTRING aniName = aniActive->protoAni->aniName;
      if (StringContains(aniName, "S_1H") || StringContains(aniName, "S_2H") ||
          StringContains(aniName, "T_1H") || StringContains(aniName, "T_2H")) {
        if (StringContains(aniName, "ATTACK") || StringContains(aniName, "COMBO")) {
          return true;
        }
      }
    }
    return false;
  }

  void CheckWeaponWaterSplash() {
    if (!configWeaponEffects) return;
    if (!IsWorldReady() || !ogame || !ogame->GetWorld()) return;
    
    oCNpc* hero = ogame->GetSelfPlayerVob();
    if (!hero) return;
    
    bool isSwinging = IsInMeleeAttackAni(hero);
    float currentTime = ztimer->totalTimeFloat;
    
    if (isSwinging && (currentTime - lastWeaponSplashTime > WEAPON_SPLASH_COOLDOWN)) {
      zVEC3 weaponPos;
      zCModel* model = hero->GetModel();
      
      if (model) {
        zCModelNodeInst* rightHandNode = model->SearchNode(zSTRING("ZS_RIGHTHAND"));
        if (rightHandNode) {
          weaponPos = model->GetNodePositionWorld(rightHandNode);
        } else {
          weaponPos = hero->GetPositionWorld();
          weaponPos[VY] += 100.0f;
        }
      } else {
        weaponPos = hero->GetPositionWorld();
        weaponPos[VY] += 100.0f;
      }
      
      float waterLevel = GetWaterLevelAt(weaponPos);
      
      if (waterLevel > -99000.0f) {
        float distToWater = weaponPos[VY] - waterLevel;
        if (distToWater < 100.0f && distToWater > -200.0f) {
          zVEC3 splashPos = weaponPos;
          splashPos[VY] = waterLevel + 5.0f;
          oCVisualFX::CreateAndPlay(zSTRING("spellFX_WaterSplash"), splashPos, nullptr, 0, 0.0f, 0, 0);
          lastWeaponSplashTime = currentTime;
        }
      }
    }
  }

  void InstallVtableHooks() {
    DWORD* vtableArrow = nullptr;
    #if ENGINE == Engine_G1
    vtableArrow = (DWORD*)0x007DC9A4;
    #elif ENGINE == Engine_G1A
    vtableArrow = (DWORD*)0x0081F8D0;
    #elif ENGINE == Engine_G2
    vtableArrow = (DWORD*)0x0082CF08;
    #elif ENGINE == Engine_G2A
    vtableArrow = (DWORD*)0x0083BFDC;
    #endif
    
    if (vtableArrow) {
      oCAIArrow_DoAI_Org = (void (__thiscall*)(oCAIArrow*, zCVob*, int&))vtableArrow[4];
      oCAIArrow_ReportCollision_Org = (void (__thiscall*)(oCAIArrow*, zCCollisionReport const&))vtableArrow[6];
      
      DWORD oldProtect;
      VirtualProtect(&vtableArrow[4], sizeof(DWORD) * 3, PAGE_EXECUTE_READWRITE, &oldProtect);
      vtableArrow[4] = (DWORD)oCAIArrow_DoAI_Hook;
      vtableArrow[6] = (DWORD)oCAIArrow_ReportCollision_Hook;
      VirtualProtect(&vtableArrow[4], sizeof(DWORD) * 3, oldProtect, &oldProtect);
    }
  }

  void Game_Entry() {}
  void Game_Init() { LoadConfiguration(); InstallVtableHooks(); }
  void Game_Exit() { trackedVobs.EmptyList(); splashedVobs.EmptyList(); }
  void Game_PreLoop() {}
  void Game_Loop() { CheckWeaponWaterSplash(); UpdateDroppedItems(); }
  void Game_PostLoop() {}
  void Game_MenuLoop() {}

  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {}
  void Game_SaveEnd() {}

  void LoadBegin() {
    trackedVobs.EmptyList();
    splashedVobs.EmptyList();
    droppedItems.EmptyList();
    lastWeaponSplashTime = 0.0f;
  }

  void LoadEnd() {}
  void Game_LoadBegin_NewGame() { LoadBegin(); }
  void Game_LoadEnd_NewGame() { LoadEnd(); }
  void Game_LoadBegin_SaveGame() { LoadBegin(); }
  void Game_LoadEnd_SaveGame() { LoadEnd(); }
  void Game_LoadBegin_ChangeLevel() { LoadBegin(); }
  void Game_LoadEnd_ChangeLevel() { LoadEnd(); }
  void Game_LoadBegin_Trigger() {}
  void Game_LoadEnd_Trigger() {}
  void Game_Pause() {}
  void Game_Unpause() {}
  void Game_DefineExternals() {}
  void Game_ApplyOptions() { LoadConfiguration(); }

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_PreLoop,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_PostLoop,
    Enabled( AppDefault ) Game_MenuLoop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals,
    Enabled( AppDefault ) Game_ApplyOptions
  );
}