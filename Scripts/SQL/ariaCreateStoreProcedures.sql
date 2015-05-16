/*
Created: 06/06/2013
Modified: 23/07/2014
Project: ARIA
Model: AriaPhysicalModel
Company: AREVA
Author: Beauté Ivan
Version: 0.2
Database: MySQL 5.5
*/

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

DELIMITER $$


-- Drop functions section --------------------------------------------------- 

DROP FUNCTION IF EXISTS `FcSgGeometricZoneExist`
;
DROP FUNCTION IF EXISTS `FcTubeIsToControl`
;
DROP FUNCTION IF EXISTS `FcExamAttributeExist`
;
DROP FUNCTION IF EXISTS `FcControlTubeExist`
;
DROP FUNCTION IF EXISTS `FcUnitExist`
;
DROP FUNCTION IF EXISTS `FcUnitConfigurationExist`
;
DROP FUNCTION IF EXISTS `FcActorExist`
;
DROP FUNCTION IF EXISTS `FcTaskExist`
;
DROP FUNCTION IF EXISTS `FcInspectionAttributeExist`
;
DROP FUNCTION IF EXISTS `FcDatabaseVersionExist`
;
DROP FUNCTION IF EXISTS `FcTechniqueEquipmentTypeExist`
;
DROP FUNCTION IF EXISTS `FcTechniqueAttributeExist`
;
DROP FUNCTION IF EXISTS `FcQueueFileTransferExist`
;
DROP FUNCTION IF EXISTS `FcBackupHistoryExist`
;
DROP FUNCTION IF EXISTS `FcPokActionExist`
;
DROP FUNCTION IF EXISTS `FcMessageExist`
;
DROP FUNCTION IF EXISTS `FcInspectionSelectedControlExist`
;
DROP FUNCTION IF EXISTS `FcEventExist`
;
DROP FUNCTION IF EXISTS `FcControlSelectedEquipmentExist`
;
DROP FUNCTION IF EXISTS `FcTubeZoneExist`
;
DROP FUNCTION IF EXISTS `FcSgTubeGeometricZoneExist`
;
DROP FUNCTION IF EXISTS `FcSgTubeExist`
;
DROP FUNCTION IF EXISTS `FcOrganAttributeExist`
;
DROP FUNCTION IF EXISTS `FcZoneExist`
;
DROP FUNCTION IF EXISTS `FcTubeGeometricZoneExist`
;
DROP FUNCTION IF EXISTS `FcTubeExist`
;
DROP FUNCTION IF EXISTS `FcTechniqueExist`
;
DROP FUNCTION IF EXISTS `FcSteamGeneratorTypeExist`
;
DROP FUNCTION IF EXISTS `FcSiteExist`
;
DROP FUNCTION IF EXISTS `FcQcResultValueExist`
;
DROP FUNCTION IF EXISTS `FcQcResultExist`
;
DROP FUNCTION IF EXISTS `FcOrganExist`
;
DROP FUNCTION IF EXISTS `FcNuclearPlantUnitExist`
;
DROP FUNCTION IF EXISTS `FcManwayExist`
;
DROP FUNCTION IF EXISTS `FcLayoutExist`
;
DROP FUNCTION IF EXISTS `FcIOExist`
;
DROP FUNCTION IF EXISTS `FcInspectionExist`
;
DROP FUNCTION IF EXISTS `FcGeometricZoneExist`
;
DROP FUNCTION IF EXISTS `FcEquipmentTypeExist`
;
DROP FUNCTION IF EXISTS `FcTextExist`
;
DROP FUNCTION IF EXISTS `FcLanguageExist`
;
DROP FUNCTION IF EXISTS `FcExamExist`
;
DROP FUNCTION IF EXISTS `FcUserExist`
;
DROP FUNCTION IF EXISTS `FcRoleExist`
;
DROP FUNCTION IF EXISTS `FcEquipmentTrackingAttributeExist`
;
DROP FUNCTION IF EXISTS `FcAcquisitionGroupExist`
;
DROP FUNCTION IF EXISTS `FcAcquisitionExist`
;
DROP FUNCTION IF EXISTS `FcAcquisitionLineExist`
;
DROP FUNCTION IF EXISTS `FcAcquisitionFileExist`
;
DROP FUNCTION IF EXISTS `FcFileExist`
;
DROP FUNCTION IF EXISTS `FcEquipmentTypeAttributeExist`
;
DROP FUNCTION IF EXISTS `FcEquipmentTrackingExist`
;
DROP FUNCTION IF EXISTS `FcEquipmentAttributeExist`
;
DROP FUNCTION IF EXISTS `FcConverterExist`
;
DROP FUNCTION IF EXISTS `FcConversionExist`
;
DROP FUNCTION IF EXISTS `FcControlZoneExist`
;
DROP FUNCTION IF EXISTS `FcControlExist`
;
DROP FUNCTION IF EXISTS `FcControlAttributeExist`
;
DROP FUNCTION IF EXISTS `FcComponentExist`
;
DROP FUNCTION IF EXISTS `FcAttributeExist`
;
DROP FUNCTION IF EXISTS `FcApplicationFunctionalityExist`
;
DROP FUNCTION IF EXISTS `FcEquipmentExist`
;
DROP FUNCTION IF EXISTS `FcGetAttributeName`
;
DROP FUNCTION IF EXISTS `FcGetAttributeId`
;

-- Drop procedures section --------------------------------------------------- 

DROP PROCEDURE IF EXISTS `SpDeleteSteamGeneratorTypeFormula`
;
DROP PROCEDURE IF EXISTS `SpGetSteamGeneratorTypeFormulas`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSteamGeneratorTypeFormula`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateGeometricZoneType`
;
DROP PROCEDURE IF EXISTS `SpDeleteGeometricZoneType`
;
DROP PROCEDURE IF EXISTS `SpGetGeometricZoneTypes`
;
DROP PROCEDURE IF EXISTS `SpDeleteSgGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpGetSgGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteSgGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpClearSgGeometricZonesTable`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSgGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpGetInspectionStatisticAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetInspectionLabelAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesInspectionStatistic`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesInspectionLabel`
;
DROP PROCEDURE IF EXISTS `SpGetRemovalReasons`
;
DROP PROCEDURE IF EXISTS `SpGetRemovalFailureTypes`
;
DROP PROCEDURE IF EXISTS `SpGetRemovalFailureModes`
;
DROP PROCEDURE IF EXISTS `SpGetExamAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentTypeFailureModes`
;
DROP PROCEDURE IF EXISTS `SpGetControlsTubesByControl`
;
DROP PROCEDURE IF EXISTS `SpGetControlsTubes`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesExam`
;
DROP PROCEDURE IF EXISTS `SpDeleteExamAttribute`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlTube`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlsTubes`
;
DROP PROCEDURE IF EXISTS `SpClearControlsTubesTable`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateExamAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateControlTube`
;
DROP PROCEDURE IF EXISTS `SpGetUnitsInUnitConfigurations`
;
DROP PROCEDURE IF EXISTS `SpGetUnitConfigurations`
;
DROP PROCEDURE IF EXISTS `SpGetActors`
;
DROP PROCEDURE IF EXISTS `SpDeleteUnitsInUnitConfigurations`
;
DROP PROCEDURE IF EXISTS `SpDeleteUnitConfigurations`
;
DROP PROCEDURE IF EXISTS `SpDeleteUnitConfiguration`
;
DROP PROCEDURE IF EXISTS `SpDeleteActors`
;
DROP PROCEDURE IF EXISTS `SpDeleteActor`
;
DROP PROCEDURE IF EXISTS `SpClearActorsTable`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateUnitConfiguration`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateActor`
;
DROP PROCEDURE IF EXISTS `SpAddUnitsInUnitConfiguration`
;
DROP PROCEDURE IF EXISTS `SpGetPokTypes`
;
DROP PROCEDURE IF EXISTS `SpGetPokReasons`
;
DROP PROCEDURE IF EXISTS `SpGetMessageTypes`
;
DROP PROCEDURE IF EXISTS `SpDeletePokType`
;
DROP PROCEDURE IF EXISTS `SpDeletePokReason`
;
DROP PROCEDURE IF EXISTS `SpDeleteMessageType`
;
DROP PROCEDURE IF EXISTS `SpDeleteDatabaseVersion`
;
DROP PROCEDURE IF EXISTS `SpAddUpdatePokType`
;
DROP PROCEDURE IF EXISTS `SpAddUpdatePokReason`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateMessageType`
;
DROP PROCEDURE IF EXISTS `SpGetTechniqueEquipmentTypes`
;
DROP PROCEDURE IF EXISTS `SpGetTechniqueAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetQueueFilesTransfer`
;
DROP PROCEDURE IF EXISTS `SpGetDatabaseVersions`
;
DROP PROCEDURE IF EXISTS `SpDeleteTechniqueEquipmentType`
;
DROP PROCEDURE IF EXISTS `SpDeleteTechniqueAttribute`
;
DROP PROCEDURE IF EXISTS `SpDeleteQueueFileTransfer`
;
DROP PROCEDURE IF EXISTS `SpDeletePokAction`
;
DROP PROCEDURE IF EXISTS `SpDeleteMessage`
;
DROP PROCEDURE IF EXISTS `SpDeleteBackupHistory`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTechniqueEquipmentType`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTechniqueAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateQueueFileTransfer`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateInspectionAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateDatabaseVersion`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateBackupHistory`
;
DROP PROCEDURE IF EXISTS `SpGetTubesByControlZone`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesControlSteamGenerator`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesControl`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionGroupLines`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionEquipments`
;
DROP PROCEDURE IF EXISTS `SpDeleteInspectionAttribute`
;
DROP PROCEDURE IF EXISTS `SpClearQualityCheckResultValuesTable`
;
DROP PROCEDURE IF EXISTS `SpClearQualityCheckResultsTable`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateQualityCheckResultValue`
;
DROP PROCEDURE IF EXISTS `SpAddUpdatePokAction`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateMessage`
;
DROP PROCEDURE IF EXISTS `SpDeleteQualityCheckResultValues`
;
DROP PROCEDURE IF EXISTS `SpDeleteQualityCheckResultValue`
;
DROP PROCEDURE IF EXISTS `SpDeleteQualityCheckResults`
;
DROP PROCEDURE IF EXISTS `SpDeleteQualityCheckResult`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateQualityCheckResult`
;
DROP PROCEDURE IF EXISTS `SpGetQualityCheckResultValues`
;
DROP PROCEDURE IF EXISTS `SpGetQualityCheckResults`
;
DROP PROCEDURE IF EXISTS `SpGetPokActions`
;
DROP PROCEDURE IF EXISTS `SpGetMessages`
;
DROP PROCEDURE IF EXISTS `SpGetModificationDateFromLinkedTable`
;
DROP PROCEDURE IF EXISTS `SpGetInspectionSelectedControls`
;
DROP PROCEDURE IF EXISTS `SpGetInspectionAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEventsFiltered`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentTypeAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentTrackingAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentStatisticAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentStateAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentSpecificAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentsAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetCreationDateFromLinkedTable`
;
DROP PROCEDURE IF EXISTS `SpGetControlSteamGenerators`
;
DROP PROCEDURE IF EXISTS `SpGetControlSelectedEquipments`
;
DROP PROCEDURE IF EXISTS `SpGetControlAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesEquipmentType`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesEquipmentTracking`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesEquipmentStatistic`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesEquipmentState`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesEquipmentSpecific`
;
DROP PROCEDURE IF EXISTS `SpDeleteTubesZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteInspectionSelectedControl`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentTrackingsAttributes`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentAttribute`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlSteamGenerator`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlSelectedEquipment`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEvent`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentTrackingAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateControlSteamGenerator`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateControlSelectedEquipment`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAttributeEquipmentType`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAttributeEquipmentTracking`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAttributeEquipmentStatistic`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAttributeEquipmentState`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAttributeEquipmentSpecific`
;
DROP PROCEDURE IF EXISTS `SpAddInspectionSelectedControl`
;
DROP PROCEDURE IF EXISTS `SpGetZonesByTubes`
;
DROP PROCEDURE IF EXISTS `SpGetTubesByZones`
;
DROP PROCEDURE IF EXISTS `SpGetOrganAttributes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentCategories`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesSteamGenerator`
;
DROP PROCEDURE IF EXISTS `SpGetAttributesOrgan`
;
DROP PROCEDURE IF EXISTS `SpDeleteTubeZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteTubesGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteOrganAttribute`
;
DROP PROCEDURE IF EXISTS `SpDeleteIo`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentCategory`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTubesZone`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTubesGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateOrganAttribute`
;
DROP PROCEDURE IF EXISTS `SpGetEquipments`
;
DROP PROCEDURE IF EXISTS `SpGetIos`
;
DROP PROCEDURE IF EXISTS `SpGetTubeMaterials`
;
DROP PROCEDURE IF EXISTS `SpGetSgTubesGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteTubeMaterials`
;
DROP PROCEDURE IF EXISTS `SpDeleteTubeMaterial`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTubeMaterial`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentCategory`
;
DROP PROCEDURE IF EXISTS `SpSetTranslation`
;
DROP PROCEDURE IF EXISTS `SpGetModificationDate`
;
DROP PROCEDURE IF EXISTS `SpGetDictionaries`
;
DROP PROCEDURE IF EXISTS `SpGetCreationDate`
;
DROP PROCEDURE IF EXISTS `SpGetRoles`
;
DROP PROCEDURE IF EXISTS `SpGetFMECheckListItems`
;
DROP PROCEDURE IF EXISTS `SpGetBackupHistories`
;
DROP PROCEDURE IF EXISTS `SpGetApplicationFunctionalitiesRoles`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionLines`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionLines`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionLinesTable`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionGroupsLines`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionLine`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionGroupsLines`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionGroupLine`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionGroupsLinesTable`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAcquisitionLine`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAcquisitionGroup`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAcquisitionFile`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAcquisition`
;
DROP PROCEDURE IF EXISTS `SpAddAcquisitionGroupLine`
;
DROP PROCEDURE IF EXISTS `SpRaiseEnumValueNotFoundError`
;
DROP PROCEDURE IF EXISTS `SpGetUsersRoles`
;
DROP PROCEDURE IF EXISTS `SpGetTubesGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpGetTasks`
;
DROP PROCEDURE IF EXISTS `SpGetOrgans`
;
DROP PROCEDURE IF EXISTS `SpGetNuclearPlantUnits`
;
DROP PROCEDURE IF EXISTS `SpGetManways`
;
DROP PROCEDURE IF EXISTS `SpGetLayouts`
;
DROP PROCEDURE IF EXISTS `SpGetInspections`
;
DROP PROCEDURE IF EXISTS `SpGetGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpGetFiles`
;
DROP PROCEDURE IF EXISTS `SpGetExams`
;
DROP PROCEDURE IF EXISTS `SpGetEvents`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentTypes`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentTrackings`
;
DROP PROCEDURE IF EXISTS `SpGetControlZonesTubes`
;
DROP PROCEDURE IF EXISTS `SpGetControlZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteUsersRoles`
;
DROP PROCEDURE IF EXISTS `SpDeleteUserRole`
;
DROP PROCEDURE IF EXISTS `SpDeleteSgTubesGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteSgTubeGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteSgTube`
;
DROP PROCEDURE IF EXISTS `SpDeleteSectors`
;
DROP PROCEDURE IF EXISTS `SpDeleteSector`
;
DROP PROCEDURE IF EXISTS `SpDeleteRoles`
;
DROP PROCEDURE IF EXISTS `SpDeleteRole`
;
DROP PROCEDURE IF EXISTS `SpDeleteOrgans`
;
DROP PROCEDURE IF EXISTS `SpDeleteOrgan`
;
DROP PROCEDURE IF EXISTS `SpDeleteNuclearPlantUnits`
;
DROP PROCEDURE IF EXISTS `SpDeleteNuclearPlantUnit`
;
DROP PROCEDURE IF EXISTS `SpDeleteManways`
;
DROP PROCEDURE IF EXISTS `SpDeleteManway`
;
DROP PROCEDURE IF EXISTS `SpDeleteLayouts`
;
DROP PROCEDURE IF EXISTS `SpDeleteLayout`
;
DROP PROCEDURE IF EXISTS `SpDeleteLanguages`
;
DROP PROCEDURE IF EXISTS `SpDeleteInspections`
;
DROP PROCEDURE IF EXISTS `SpDeleteInspection`
;
DROP PROCEDURE IF EXISTS `SpDeleteGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteFiles`
;
DROP PROCEDURE IF EXISTS `SpDeleteFile`
;
DROP PROCEDURE IF EXISTS `SpDeleteExams`
;
DROP PROCEDURE IF EXISTS `SpDeleteExam`
;
DROP PROCEDURE IF EXISTS `SpDeleteEvents`
;
DROP PROCEDURE IF EXISTS `SpDeleteEvent`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentTypesAttributes`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentTypes`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentTypeAttribute`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentType`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentTrackings`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentTracking`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentsAttributes`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipments`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentConfiguration`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipment`
;
DROP PROCEDURE IF EXISTS `SpDeleteConversions`
;
DROP PROCEDURE IF EXISTS `SpDeleteConversion`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlZoneTube`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlZonesTubes`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlsAttributes`
;
DROP PROCEDURE IF EXISTS `SpDeleteControls`
;
DROP PROCEDURE IF EXISTS `SpDeleteControlAttribute`
;
DROP PROCEDURE IF EXISTS `SpClearFilesTable`
;
DROP PROCEDURE IF EXISTS `SpClearControlsAttributesTable`
;
DROP PROCEDURE IF EXISTS `SpAddUserRole`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSgTubeGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateFile`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentType`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentTracking`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentConfiguration`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateControlZone`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateControlAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddControlZoneTube`
;
DROP PROCEDURE IF EXISTS `SpRaiseRecordNotFoundError`
;
DROP PROCEDURE IF EXISTS `SpRaiseError`
;
DROP PROCEDURE IF EXISTS `SpGetZones`
;
DROP PROCEDURE IF EXISTS `SpGetTubes`
;
DROP PROCEDURE IF EXISTS `SpGetTechniques`
;
DROP PROCEDURE IF EXISTS `SpGetSteamGeneratorTypes`
;
DROP PROCEDURE IF EXISTS `SpGetSteamGenerators`
;
DROP PROCEDURE IF EXISTS `SpGetSites`
;
DROP PROCEDURE IF EXISTS `SpGetSgTubes`
;
DROP PROCEDURE IF EXISTS `SpGetSectors`
;
DROP PROCEDURE IF EXISTS `SpGetRecords`
;
DROP PROCEDURE IF EXISTS `SpGetEquipmentConfigurations`
;
DROP PROCEDURE IF EXISTS `SpGetComponents`
;
DROP PROCEDURE IF EXISTS `SpGetApplicationFunctionalities`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionsEquipments`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitions`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionGroups`
;
DROP PROCEDURE IF EXISTS `SpGetAcquisitionFiles`
;
DROP PROCEDURE IF EXISTS `SpDeleteZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteZone`
;
DROP PROCEDURE IF EXISTS `SpDeleteUsers`
;
DROP PROCEDURE IF EXISTS `SpDeleteUser`
;
DROP PROCEDURE IF EXISTS `SpDeleteUnits`
;
DROP PROCEDURE IF EXISTS `SpDeleteUnit`
;
DROP PROCEDURE IF EXISTS `SpDeleteTubesGeometricZones`
;
DROP PROCEDURE IF EXISTS `SpDeleteTube`
;
DROP PROCEDURE IF EXISTS `SpDeleteTexts`
;
DROP PROCEDURE IF EXISTS `SpDeleteTechniques`
;
DROP PROCEDURE IF EXISTS `SpDeleteTechnique`
;
DROP PROCEDURE IF EXISTS `SpDeleteTasks`
;
DROP PROCEDURE IF EXISTS `SpDeleteTask`
;
DROP PROCEDURE IF EXISTS `SpDeleteSteamGeneratorTypes`
;
DROP PROCEDURE IF EXISTS `SpDeleteSteamGeneratorType`
;
DROP PROCEDURE IF EXISTS `SpDeleteSteamGenerators`
;
DROP PROCEDURE IF EXISTS `SpDeleteSteamGenerator`
;
DROP PROCEDURE IF EXISTS `SpDeleteSites`
;
DROP PROCEDURE IF EXISTS `SpDeleteSite`
;
DROP PROCEDURE IF EXISTS `SpDeleteSgTubes`
;
DROP PROCEDURE IF EXISTS `SpDeleteRecords`
;
DROP PROCEDURE IF EXISTS `SpDeleteEquipmentConfigurations`
;
DROP PROCEDURE IF EXISTS `SpDeleteComponents`
;
DROP PROCEDURE IF EXISTS `SpDeleteComponent`
;
DROP PROCEDURE IF EXISTS `SpDeleteBackupHistories`
;
DROP PROCEDURE IF EXISTS `SpDeleteAttributes`
;
DROP PROCEDURE IF EXISTS `SpDeleteApplicationFunctionalityRole`
;
DROP PROCEDURE IF EXISTS `SpDeleteApplicationFunctionality`
;
DROP PROCEDURE IF EXISTS `SpDeleteApplicationFunctionalitiesRoles`
;
DROP PROCEDURE IF EXISTS `SpDeleteApplicationFunctionalities`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionsEquipments`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitions`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionGroups`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionGroup`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionFiles`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionFile`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisitionEquipment`
;
DROP PROCEDURE IF EXISTS `SpDeleteAcquisition`
;
DROP PROCEDURE IF EXISTS `SpClearUsersTable`
;
DROP PROCEDURE IF EXISTS `SpClearUnitWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearUnitsTable`
;
DROP PROCEDURE IF EXISTS `SpClearTubesTable`
;
DROP PROCEDURE IF EXISTS `SpClearTubesGeometricZonesTable`
;
DROP PROCEDURE IF EXISTS `SpClearTextsTable`
;
DROP PROCEDURE IF EXISTS `SpClearTechniqueWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearTechniquesTable`
;
DROP PROCEDURE IF EXISTS `SpClearTasksTable`
;
DROP PROCEDURE IF EXISTS `SpClearSteamGeneratorTypeWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearSteamGeneratorTypesTable`
;
DROP PROCEDURE IF EXISTS `SpClearSteamGeneratorsTable`
;
DROP PROCEDURE IF EXISTS `SpClearSitesTable`
;
DROP PROCEDURE IF EXISTS `SpClearSgTubesTable`
;
DROP PROCEDURE IF EXISTS `SpClearSgTubesGeometricZonesTable`
;
DROP PROCEDURE IF EXISTS `SpClearSecurityWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearSectorsTable`
;
DROP PROCEDURE IF EXISTS `SpClearRolesTable`
;
DROP PROCEDURE IF EXISTS `SpClearOrganWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearOrgansTable`
;
DROP PROCEDURE IF EXISTS `SpClearNuclearPlantUnitsTable`
;
DROP PROCEDURE IF EXISTS `SpClearMiscellaneousWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearManwaysTable`
;
DROP PROCEDURE IF EXISTS `SpClearLocalizationWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearLayoutsTable`
;
DROP PROCEDURE IF EXISTS `SpClearLanguagesTable`
;
DROP PROCEDURE IF EXISTS `SpClearInspectionWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearInspectionsTable`
;
DROP PROCEDURE IF EXISTS `SpClearGeometricZonesTable`
;
DROP PROCEDURE IF EXISTS `SpClearFileTransferWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearExamsTable`
;
DROP PROCEDURE IF EXISTS `SpClearEventWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearEventsTable`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentTypeWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentTypeAttributesTable`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentTrackingsTable`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentsTable`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentsAttributesTable`
;
DROP PROCEDURE IF EXISTS `SpClearEquipmentConfigurationsTable`
;
DROP PROCEDURE IF EXISTS `SpClearEAVWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearDatabase`
;
DROP PROCEDURE IF EXISTS `SpClearConversionsTable`
;
DROP PROCEDURE IF EXISTS `SpClearControlZonesTubesTable`
;
DROP PROCEDURE IF EXISTS `SpClearControlZonesTable`
;
DROP PROCEDURE IF EXISTS `SpClearControlsTable`
;
DROP PROCEDURE IF EXISTS `SpClearControlAttributesTable`
;
DROP PROCEDURE IF EXISTS `SpClearComponentsTable`
;
DROP PROCEDURE IF EXISTS `SpClearBackupHistoriesTable`
;
DROP PROCEDURE IF EXISTS `SpClearAttributesTable`
;
DROP PROCEDURE IF EXISTS `SpClearApplicationFunctionalitiesTable`
;
DROP PROCEDURE IF EXISTS `SpClearApplicationFunctionalitiesRolesTable`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionWorkspaceTables`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionsTable`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionsEquipmentsTable`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionGroupsTable`
;
DROP PROCEDURE IF EXISTS `SpClearAcquisitionFilesTable`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipment`
;
DROP PROCEDURE IF EXISTS `SpAddApplicationFunctionalityRole`
;
DROP PROCEDURE IF EXISTS `SpAddAcquisitionEquipment`
;
DROP PROCEDURE IF EXISTS `SpUnselectEquipment`
;
DROP PROCEDURE IF EXISTS `SpSelectEquipment`
;
DROP PROCEDURE IF EXISTS `SpReleaseLock`
;
DROP PROCEDURE IF EXISTS `SpLockTable`
;
DROP PROCEDURE IF EXISTS `SpGetSelectedEquipments`
;
DROP PROCEDURE IF EXISTS `SpClearSelectedEquipment`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateConversion`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateComponent`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateExam`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateZone`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateUser`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTube`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTechnique`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateTask`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSteamGeneratorType`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSteamGenerator`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSite`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSgTube`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateSector`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateRole`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateOrgan`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateNuclearPlantUnit`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateManway`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateLayout`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateIO`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateInspection`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateGeometricZone`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateEquipmentTypeAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateApplicationFunctionality`
;
DROP PROCEDURE IF EXISTS `SpGetUsers`
;
DROP PROCEDURE IF EXISTS `SpGetUserRoles`
;
DROP PROCEDURE IF EXISTS `SpGetUnits`
;
DROP PROCEDURE IF EXISTS `SpGetRoleFunctionalities`
;
DROP PROCEDURE IF EXISTS `SpGetLanguages`
;
DROP PROCEDURE IF EXISTS `SpGetConversions`
;
DROP PROCEDURE IF EXISTS `SpDeleteText`
;
DROP PROCEDURE IF EXISTS `SpDeleteLanguage`
;
DROP PROCEDURE IF EXISTS `SpClearLocalizationTables`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateUnit`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateLanguage`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateText`
;
DROP PROCEDURE IF EXISTS `SpGetTexts`
;
DROP PROCEDURE IF EXISTS `SpGetAttributes`
;
DROP PROCEDURE IF EXISTS `SpDeleteAttribute`
;
DROP PROCEDURE IF EXISTS `SpAddUpdateAttribute`
;

-- Create procedures section -------------------------------------------------

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAttribute`
(
INOUT id BIGINT,
IN parentId BIGINT,
IN relatedId BIGINT,
IN unitId BIGINT,
IN dataType INT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN description TEXT,
IN isNotNull TINYINT(1),
IN defaultValue VARCHAR(255),
IN value1 VARCHAR(255),
IN value2 VARCHAR(255),
IN valueFormat VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record'
BEGIN
 

    DECLARE parentIdTmp BIGINT;
    DECLARE fullName VARCHAR(500) CHARSET utf8 DEFAULT NULL;

	SET parentIdTmp = parentId;
	SET fullName = name;

	WHILE parentIdTmp IS NOT NULL DO
       SET fullName = CONCAT( FcGetAttributeName( parentIdTmp ), '.', fullName );

	   SELECT ATTRIBUTE_PARENT_ID
	   INTO parentIdTmp
	   FROM ATTRIBUTES
	   WHERE ATTRIBUTE_ID = parentIdTmp;

    END WHILE;

    IF unitId <= 0 THEN
        SET unitId = NULL;
    END IF;

    IF relatedId <= 0 THEN
        SET relatedId = NULL;
    END IF;


    IF id IS NULL THEN

        INSERT INTO ATTRIBUTES 
        (
            ATTRIBUTE_UNIT_ID,
            ATTRIBUTE_PARENT_ID,
            ATTRIBUTE_RELATED_ID,
            ATTRIBUTE_DATA_TYPE,
            ATTRIBUTE_NAME,
            ATTRIBUTE_FULL_NAME,
            ATTRIBUTE_LABEL,
            ATTRIBUTE_LABEL_TRANSLATION_KEY,
            ATTRIBUTE_DESCRIPTION,
            ATTRIBUTE_NOT_NULL,
            ATTRIBUTE_DEFAULT_VALUE,
            ATTRIBUTE_VALUE1,
            ATTRIBUTE_VALUE2,
            ATTRIBUTE_VALUE_FORMAT,
            ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            unitId,
            parentId,
            relatedId,
            dataType,
            name,
            fullName,
            label,
            labelTranslationKey,
            description,
            isNotNull,
            defaultValue,
            value1,
            value2,
            valueFormat,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ATTRIBUTES', 'ATTRIBUTE',  id, date );

    ELSE
    
        IF NOT FcAttributeExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ATTRIBUTES', id );
        END IF;

        UPDATE 
            ATTRIBUTES 
        SET
            ATTRIBUTE_UNIT_ID = unitId,
            ATTRIBUTE_PARENT_ID = parentId,
            ATTRIBUTE_RELATED_ID = relatedId,
            ATTRIBUTE_DATA_TYPE = dataType,
            ATTRIBUTE_NAME = name,
            ATTRIBUTE_FULL_NAME = fullName,
            ATTRIBUTE_LABEL = label,
            ATTRIBUTE_LABEL_TRANSLATION_KEY = labelTranslationKey,
            ATTRIBUTE_DESCRIPTION = description,
            ATTRIBUTE_NOT_NULL = isNotNull,
            ATTRIBUTE_DEFAULT_VALUE = defaultValue,
            ATTRIBUTE_VALUE1 = value1,
            ATTRIBUTE_VALUE2 = value2,
            ATTRIBUTE_VALUE_FORMAT = valueFormat,
            ATTRIBUTE_MODIFICATION_DATE = NOW()
        WHERE
            ATTRIBUTE_ID = id;
            
        CALL SpGetModificationDate('ATTRIBUTES', 'ATTRIBUTE',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAttribute`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    DELETE FROM 
        ATTRIBUTES
    WHERE
        ATTRIBUTE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ATTRIBUTES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_PARENT_ID AS PARENT_ID,
                            A.ATTRIBUTE_RELATED_ID AS RELATED_ID,
                            A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
                            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
                            A.ATTRIBUTE_NOT_NULL AS IS_NOT_NULL,
                            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
                            A.ATTRIBUTE_VALUE1 AS VALUE1,
                            A.ATTRIBUTE_VALUE2 AS VALUE2,
                            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A'
                    ,whereClause );  

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTexts`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all record present in the TEXTS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            T.TEXT_ID AS TEXT_ID,
                            T.TEXT_LANGUAGE_ID AS LANGUAGE_ID,
                            T.TEXT_COMPONENT_ID AS COMPONENT_ID,
                            T.TEXT_GROUP_ID AS GROUP_ID,
                            L.LANGUAGE_NAME AS LANGUAGE_NAME,
                            C.COMPONENT_NAME AS COMPONENT_NAME,
                            A.ATTRIBUTE_NAME AS GROUP_NAME,
                            T.TEXT_KEY AS TRANSLATION_KEY,
                            T.TEXT_VALUE AS VALUE,
                            T.TEXT_ERROR_CODE AS ERROR_CODE,
                            T.TEXT_GROUP_CODE AS GROUP_CODE
                        FROM
                            TEXTS T
                                INNER JOIN LANGUAGES L ON L.LANGUAGE_ID = T.TEXT_LANGUAGE_ID
                                LEFT JOIN COMPONENTS C ON C.COMPONENT_ID = T.TEXT_COMPONENT_ID
                                LEFT JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = T.TEXT_GROUP_ID '
                    ,whereClause );
                    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateText`
(INOUT id BIGINT,
IN componentId BIGINT,
IN groupId BIGINT,
IN languageId BIGINT,
IN translationKey VARCHAR(100),
IN value TEXT,
IN errorCode VARCHAR(10),
IN groupCode VARCHAR(10))
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TEXTS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO TEXTS 
        (
            TEXT_LANGUAGE_ID,
            TEXT_COMPONENT_ID,
            TEXT_GROUP_ID,
            TEXT_KEY,
            TEXT_VALUE,
            TEXT_ERROR_CODE,
            TEXT_GROUP_CODE,
            TEXT_CREATION_DATE
        )
        VALUES
        (
            languageId,
            componentId,
            groupId,
            translationKey,
            value,
            errorCode,
            groupCode,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();

    ELSE
    
        IF NOT FcTextExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'TEXTS', id );
        END IF;

        UPDATE 
            TEXTS 
        SET
            TEXT_LANGUAGE_ID = languageId,
            TEXT_COMPONENT_ID = componentId,
            TEXT_GROUP_ID = groupId,
            TEXT_KEY = translationKey,
            TEXT_VALUE = value,
            TEXT_ERROR_CODE = errorCode,
            TEXT_GROUP_CODE = groupCode,
            TEXT_MODIFICATION_DATE = NOW()
        WHERE
            TEXT_ID = id;

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateLanguage`
(
INOUT id BIGINT,
IN name VARCHAR(5),
IN description VARCHAR(100),
OUT date DATETIME)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the LANGUAGES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO LANGUAGES 
        (
            LANGUAGE_NAME,
            LANGUAGE_DESCRIPTION,
            LANGUAGE_CREATION_DATE
        )
        VALUES
        (
            name,
            description,
            NOW()
        );
    
        SET id = LAST_INSERT_ID();     
        
        CALL SpGetCreationDate('LANGUAGES', 'LANGUAGE',  id, date );

    ELSE
    
        IF NOT FcLanguageExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'LANGUAGES', id );
        END IF;

        UPDATE 
            LANGUAGES 
        SET
            LANGUAGE_NAME = name,
            LANGUAGE_DESCRIPTION = description,
            LANGUAGE_MODIFICATION_DATE = NOW()
        WHERE
            LANGUAGE_ID = id;     
            
        CALL SpGetModificationDate('LANGUAGES', 'LANGUAGE',  id, date );

    END IF;
  
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateUnit`
(
INOUT id BIGINT,
IN unitDimension SMALLINT,
IN unitName VARCHAR(45),
IN unitSymbol VARCHAR(10),
OUT date DATETIME)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the UNITS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO UNITS 
        (
            UNIT_DIMENSION,
            UNIT_NAME,
            UNIT_SYMBOL,
            UNIT_CREATION_DATE
        )
        VALUES
        (
            unitDimension,
            unitName,
            unitSymbol,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('UNITS', 'UNIT',  id, date );

    ELSE
    
        IF NOT FcUnitExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'UNITS', id );
        END IF;

        UPDATE 
            UNITS 
        SET
            UNIT_DIMENSION = unitDimension,
            UNIT_NAME = unitName,
            UNIT_SYMBOL = unitSymbol,
            UNIT_MODIFICATION_DATE = NOW()
        WHERE
            UNIT_ID = id;
            
        CALL SpGetModificationDate('UNITS', 'UNIT',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearLocalizationTables`
()
SQL SECURITY DEFINER
COMMENT 'Clear the tables : TEXTS, LANGUAGES'
BEGIN
    CALL SpDeleteAllTexts();
    CALL SpDeleteAllLanguages();
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteLanguage`
(
IN id BIGINT)
SQL SECURITY DEFINER
BEGIN
    DELETE FROM 
        LANGUAGES
    WHERE
        LANGUAGE_ID = id;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteText`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the TEXTS table'
BEGIN

    DELETE FROM
        TEXTS
    WHERE
        TEXT_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetConversions`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONVERSIONS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            CS.CONVERSION_UNIT_SOURCE_ID AS UNIT_SOURCE_ID,
                            CS.CONVERSION_UNIT_DEST_ID AS UNIT_DEST_ID,
                            US.UNIT_DIMENSION AS UNIT_SOURCE_DIMENSION,
                            UD.UNIT_DIMENSION AS UNIT_DEST_DIMENSION,
                            CS.CONVERSION_CONVERTER_TYPE AS CONVERTER_TYPE,
                            US.UNIT_NAME AS UNIT_SOURCE_NAME,
                            US.UNIT_SYMBOL AS UNIT_SOURCE_SYMBOL,
                            UD.UNIT_NAME AS UNIT_DEST_NAME,
                            UD.UNIT_SYMBOL AS UNIT_DEST_SYMBOL,
                            CS.CONVERSION_PARAMETERS AS PARAMETERS,
                            CS.CONVERSION_CREATION_DATE AS CREATION_DATE,
                            CS.CONVERSION_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            CONVERSIONS CS
                                INNER JOIN UNITS US ON US.UNIT_ID = CS.CONVERSION_UNIT_SOURCE_ID
                                INNER JOIN UNITS UD ON UD.UNIT_ID = CS.CONVERSION_UNIT_DEST_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetLanguages`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

    SET @strQuery = CONCAT( 'SELECT
                                L.LANGUAGE_ID AS Id,
                                L.LANGUAGE_NAME AS Name,
                                L.L.LANGUAGE_DESCRIPTION AS Description
                             FROM
                                LANGUAGES L ',
                        whereClause );

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetRoleFunctionalities`
(
IN roleId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the APPLICATION_FUNCTIONALITIES_ROLES table matching with the given roleId.'
BEGIN

    SELECT
        AFR.AFR_APPLICATION_FUNCTIONALITY_ID AS ID,
        AFR.AFR_ROLE_ID AS ROLE_ID,
        R.ROLE_NAME AS ROLE_NAME,
        R.ROLE_LEVEL AS ROLE_LEVEL,
        AF.APPLICATION_FUNCTIONALITY_CODE AS CODE,
        AF.APPLICATION_FUNCTIONALITY_DESCRIPTION AS DESCRIPTION,
        AF.APPLICATION_FUNCTIONALITY_CREATION_DATE AS CREATION_DATE,
        AF.APPLICATION_FUNCTIONALITY_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        APPLICATION_FUNCTIONALITIES_ROLES AFR
            INNER JOIN APPLICATION_FUNCTIONALITIES AF ON AFR.AFR_APPLICATION_FUNCTIONALITY_ID = AF.APPLICATION_FUNCTIONALITY_ID
            INNER JOIN ROLES R ON AFR.AFR_ROLE_ID = R.ROLE_ID 
    WHERE
        AFR.AFR_ROLE_ID = roleId
    ORDER BY 
        AF.APPLICATION_FUNCTIONALITY_CODE ASC;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetUnits`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

    CALL SpGetRecords( 'SELECT
                            U.UNIT_ID AS ID,
                            U.UNIT_DIMENSION AS DIMENSION,
                            U.UNIT_NAME AS NAME,
                            U.UNIT_SYMBOL AS SYMBOL,
                            U.UNIT_CREATION_DATE AS CREATION_DATE,
                            U.UNIT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            UNITS U'
                    ,whereClause );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetUserRoles`
(
IN userId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the USERS_ROLES table corresponding to the specified user'
BEGIN

    SELECT
        UR.USER_ID AS USER_ID,
        UR.ROLE_ID AS ID,
        U.USER_LOGIN AS USER_LOGIN,
        R.ROLE_NAME AS NAME,
        R.ROLE_LEVEL AS LEVEL,
        R.ROLE_CREATION_DATE AS CREATION_DATE,
        R.ROLE_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        USERS_ROLES UR
            INNER JOIN ROLES R ON UR.ROLE_ID = R.ROLE_ID
            INNER JOIN USERS U ON UR.USER_ID = U.USER_ID 
    WHERE
        UR.USER_ID = userId
    ORDER BY 
        R.ROLE_NAME ASC;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetUsers`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the USERS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            U.USER_ID AS ID,
                            U.USER_CERTIFICATION_LEVEL AS CERTIFICATION_LEVEL,
                            U.USER_LOGIN AS LOGIN,
                            U.USER_HASH_PASSWORD AS HASH_PASSWORD,
                            U.USER_LAST_NAME AS LAST_NAME,
                            U.USER_FIRST_NAME AS FIRST_NAME,
                            U.USER_COMPANY AS COMPANY,
                            U.USER_IMAGE AS IMAGE,
                            U.USER_IS_ROOT AS IS_ROOT,
						    U.USER_CREATION_DATE AS CREATION_DATE,
                            U.USER_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            USERS U'
                        ,whereClause );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateApplicationFunctionality`
(
INOUT id BIGINT,
IN code CHAR(10),
IN description VARCHAR(255),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the APPLICATION_FUNCTIONALITIES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO APPLICATION_FUNCTIONALITIES 
        (
            APPLICATION_FUNCTIONALITY_CODE,
            APPLICATION_FUNCTIONALITY_DESCRIPTION,
            APPLICATION_FUNCTIONALITY_CREATION_DATE
        )
        VALUES
        (
            code,
            description,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('APPLICATION_FUNCTIONALITIES', 'APPLICATION_FUNCTIONALITY',  id, date );

    ELSE
    
        IF NOT FcApplicationFunctionalityExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'APPLICATION_FUNCTIONALITIES', id );
        END IF;

        UPDATE 
            APPLICATION_FUNCTIONALITIES 
        SET
            APPLICATION_FUNCTIONALITY_CODE = code,
            APPLICATION_FUNCTIONALITY_DESCRIPTION = description,
            APPLICATION_FUNCTIONALITY_MODIFICATION_DATE = NOW()
        WHERE
            APPLICATION_FUNCTIONALITY_ID = id;
            
        CALL SpGetModificationDate('APPLICATION_FUNCTIONALITIES', 'APPLICATION_FUNCTIONALITY',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentTypeAttribute`
(
IN equipmentTypeId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENT_TYPES_ATTRIBUTES table or update an existing record'
BEGIN

    IF FcEquipmentTypeAttributeExist( equipmentTypeId, attributeId ) = FALSE THEN
  
        INSERT INTO EQUIPMENT_TYPES_ATTRIBUTES 
        (
            ETA_EQUIPMENT_TYPE_ID,
            ETA_ATTRIBUTE_ID,
            ETA_VALUE,
            ETA_CREATION_DATE
        )
        VALUES
        (
            equipmentTypeId,
            attributeId,
            value,
            NOW()
        );

        CALL SpGetCreationDateFromLinkedTable('EQUIPMENT_TYPES_ATTRIBUTES', 'ETA',  CONCAT( ' WHERE ETA_EQUIPMENT_TYPE_ID = ', equipmentTypeId, ' AND ETA_ATTRIBUTE_ID = ', attributeId), date );
    ELSE
    
        UPDATE
            EQUIPMENT_TYPES_ATTRIBUTES 
        SET
            ETA_VALUE = value,
            ETA_MODIFICATION_DATE = NOW()
        WHERE
            ETA_EQUIPMENT_TYPE_ID = equipmentTypeId
        AND
            ETA_ATTRIBUTE_ID = attributeId;
    
	    CALL SpGetModificationDateFromLinkedTable('EQUIPMENT_TYPES_ATTRIBUTES', 'ETA', CONCAT( ' WHERE ETA_EQUIPMENT_TYPE_ID = ', equipmentTypeId, ' AND ETA_ATTRIBUTE_ID = ', attributeId), date );
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateGeometricZone`
(
INOUT id BIGINT,
IN steamGeneratorTypeId BIGINT,
IN typeId BIGINT,
IN name VARCHAR(30),
IN length DOUBLE,
IN srzValue INT,
IN limitPlus FLOAT,
IN limitMinus FLOAT,
IN overlap FLOAT,
IN signChange TINYINT(1),
IN reportRef TINYINT(1),
IN legType SMALLINT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the GEOMETRIC_ZONES table or update an existing record'
BEGIN
	
    IF id IS NULL THEN

        INSERT INTO GEOMETRIC_ZONES 
        (
            GEOMETRIC_ZONE_STEAM_GENERATOR_TYPE_ID,
            GEOMETRIC_ZONE_TYPE_ID,
            GEOMETRIC_ZONE_NAME,
            GEOMETRIC_ZONE_LENGTH,
            GEOMETRIC_ZONE_SRZ_VALUE,  
            GEOMETRIC_ZONE_LIMIT_PLUS,
            GEOMETRIC_ZONE_LIMIT_MINUS,
            GEOMETRIC_ZONE_OVERLAP, 
            GEOMETRIC_ZONE_SIGN_CHANGE,
            GEOMETRIC_ZONE_REPORT_REF,
            GEOMETRIC_ZONE_LEG_TYPE,
            GEOMETRIC_ZONE_CREATION_DATE
        )
        VALUES
        (
            steamGeneratorTypeId,
            typeId,
            name,
            length,
            srzValue,
            limitPlus,
            limitMinus,
            overlap,
            signChange,
            reportRef,
            legType,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('GEOMETRIC_ZONES', 'GEOMETRIC_ZONE',  id, date );

    ELSE
    
        IF NOT FcGeometricZoneExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'GEOMETRIC_ZONES', id );
        END IF;

        UPDATE 
            GEOMETRIC_ZONES 
        SET
            GEOMETRIC_ZONE_STEAM_GENERATOR_TYPE_ID = steamGeneratorTypeId,
            GEOMETRIC_ZONE_TYPE_ID = typeId,
            GEOMETRIC_ZONE_NAME = name,
            GEOMETRIC_ZONE_LENGTH = length,
            GEOMETRIC_ZONE_SRZ_VALUE = srzValue,
            GEOMETRIC_ZONE_LIMIT_PLUS =limitPlus,
            GEOMETRIC_ZONE_LIMIT_MINUS =limitMinus,
            GEOMETRIC_ZONE_OVERLAP =overlap, 
            GEOMETRIC_ZONE_SIGN_CHANGE =signChange,
            GEOMETRIC_ZONE_REPORT_REF =reportRef,
            GEOMETRIC_ZONE_LEG_TYPE =legType,
            GEOMETRIC_ZONE_MODIFICATION_DATE = NOW()
        WHERE
            GEOMETRIC_ZONE_ID = id;
            
        CALL SpGetModificationDate('GEOMETRIC_ZONES', 'GEOMETRIC_ZONE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateInspection`
(
INOUT id BIGINT,
IN siteId BIGINT,
IN contractorId BIGINT,
IN nuclearPlantUnitId BIGINT,
IN unitConfigurationId BIGINT,
IN name VARCHAR(50),
IN number VARCHAR(50),
IN startDate DATETIME,
IN finishDate DATETIME,
IN comment TEXT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the INSPECTIONS table or update an existing record'
BEGIN

	IF nuclearPlantUnitId <=0 THEN
	   SET nuclearPlantUnitId = null;
	END IF;

    IF id IS NULL THEN

        INSERT INTO INSPECTIONS 
        (
            INSPECTION_SITE_ID,
	    INSPECTION_CONTRACTOR_ID,
            INSPECTION_NUCLEAR_PLANT_UNIT_ID,
            INSPECTION_UNIT_CONFIGURATION_ID,
            INSPECTION_NAME,
            INSPECTION_NUMBER,
            INSPECTION_START_DATE,
            INSPECTION_FINISH_DATE,
            INSPECTION_COMMENT,
            INSPECTION_CREATION_DATE
        )
        VALUES
        (
            siteId,
	    contractorId,
            nuclearPlantUnitId,
            unitConfigurationId,
            name,
            number,
            startDate,
            finishDate,
            comment,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('INSPECTIONS', 'INSPECTION',  id, date );

    ELSE
    
        IF NOT FcInspectionExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'INSPECTIONS', id );
        END IF;

        UPDATE 
            INSPECTIONS 
        SET
            INSPECTION_SITE_ID = siteId,
	    INSPECTION_CONTRACTOR_ID = contractorId,
            INSPECTION_NUCLEAR_PLANT_UNIT_ID = nuclearPlantUnitId,
            INSPECTION_UNIT_CONFIGURATION_ID = unitConfigurationId,
            INSPECTION_NAME = name,
            INSPECTION_NUMBER = number,
            INSPECTION_START_DATE = startDate,
            INSPECTION_FINISH_DATE = finishDate,
            INSPECTION_COMMENT = comment,
            INSPECTION_MODIFICATION_DATE = NOW()
        WHERE
            INSPECTION_ID = id;
            
        CALL SpGetModificationDate('INSPECTIONS', 'INSPECTION',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateIO`
(
INOUT id BIGINT,
IN equipmentTypeId BIGINT,
IN unitId BIGINT,
IN dataType SMALLINT,
IN name VARCHAR(30),
IN isInput TINYINT(1),
IN technicalAddress VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the IOS table or update an existing record'
BEGIN

    IF unitId <=0 THEN
	   SET unitId = null;
	END IF;

    IF id IS NULL THEN

        INSERT INTO IOS 
        (
            IO_DATA_TYPE,
            IO_EQUIPMENT_TYPE_ID,
            IO_UNIT_ID,
            IO_NAME,
            IO_IS_INPUT,
            IO_TECHNICAL_ADDRESS,
            IO_CREATION_DATE
        )
        VALUES
        (
            dataType,
            equipmentTypeId,
            unitId,
            name,
            isInput,
            technicalAddress,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('IOS', 'IO',  id, date );

    ELSE
    
        IF NOT FcIOExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'IOS', id );
        END IF;

        UPDATE 
            IOS 
        SET
            IO_DATA_TYPE = dataType,
            IO_EQUIPMENT_TYPE_ID = equipmentTypeId,
            IO_UNIT_ID = unitId,
            IO_NAME = name,
            IO_IS_INPUT = isInput,
            IO_TECHNICAL_ADDRESS = technicalAddress,
            IO_MODIFICATION_DATE = NOW()
        WHERE
            IO_ID = id;
            
        CALL SpGetModificationDate('IOS', 'IO',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateLayout`
(
INOUT id BIGINT,
IN name VARCHAR(50),
IN file VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the LAYOUTS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO LAYOUTS 
        (
            LAYOUT_NAME,
            LAYOUT_FILE,
            LAYOUT_CREATION_DATE
        )
        VALUES
        (
            name,
            file,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('LAYOUTS', 'LAYOUT',  id, date );

    ELSE
    
        IF NOT FcLayoutExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'LAYOUTS', id );
        END IF;

        UPDATE 
            LAYOUTS 
        SET
            LAYOUT_NAME = name,
            LAYOUT_FILE = file,
            LAYOUT_MODIFICATION_DATE = NOW()
        WHERE
            LAYOUT_ID = id;
            
        CALL SpGetModificationDate('LAYOUTS', 'LAYOUT',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateManway`
(
INOUT id BIGINT,
IN steamGeneratorId BIGINT,
IN legType INT,
IN diameter DOUBLE,
IN rowPosition INT,
IN columnPosition INT,
IN xCoordinate FLOAT,
IN yCoordinate FLOAT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the MANWAYS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO MANWAYS 
        (
            MANWAY_STEAM_GENERATOR_ID,
            MANWAY_LEG_TYPE,
            MANWAY_DIAMETER,
            MANWAY_ROW,
            MANWAY_COLUMN,
	    MANWAY_X,
	    MANWAY_Y,
            MANWAY_CREATION_DATE
        )
        VALUES
        (
            steamGeneratorId,
            legType,
            diameter,
            rowPosition,
            columnPosition,
	    xCoordinate,
	    yCoordinate,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('MANWAYS', 'MANWAY',  id, date );

    ELSE
    
        IF NOT FcManwayExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'MANWAYS', id );
        END IF;

        UPDATE 
            MANWAYS 
        SET
            MANWAY_STEAM_GENERATOR_ID = steamGeneratorId,
            MANWAY_LEG_TYPE = legType,
            MANWAY_DIAMETER = diameter,
            MANWAY_ROW = rowPosition,
            MANWAY_COLUMN = columnPosition,
	    MANWAY_X = xCoordinate,
	    MANWAY_Y = yCoordinate,
            MANWAY_MODIFICATION_DATE = NOW()
        WHERE
            MANWAY_ID = id;
            
        CALL SpGetModificationDate('MANWAYS', 'MANWAY',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateNuclearPlantUnit`
(
INOUT id BIGINT,
IN siteId BIGINT,
IN number INT,
IN name VARCHAR(10),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the NUCLEAR_PLANT_UNITS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO NUCLEAR_PLANT_UNITS 
        (
            NUCLEAR_PLANT_UNIT_SITE_ID,
            NUCLEAR_PLANT_UNIT_NUMBER,
            NUCLEAR_PLANT_UNIT_NAME,
            NUCLEAR_PLANT_UNIT_CREATION_DATE
        )
        VALUES
        (
            siteId,
            number,
            name,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('NUCLEAR_PLANT_UNITS', 'NUCLEAR_PLANT_UNIT',  id, date );

    ELSE
        
        IF NOT FcNuclearPlantUnitExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'NUCLEAR_PLANT_UNITS', id );
        END IF;

        UPDATE 
            NUCLEAR_PLANT_UNITS 
        SET
            NUCLEAR_PLANT_UNIT_SITE_ID = siteId,
            NUCLEAR_PLANT_UNIT_NUMBER = number,
            NUCLEAR_PLANT_UNIT_NAME = name,
            NUCLEAR_PLANT_UNIT_MODIFICATION_DATE = NOW()
        WHERE
            NUCLEAR_PLANT_UNIT_ID = id;
            
        CALL SpGetModificationDate('NUCLEAR_PLANT_UNITS', 'NUCLEAR_PLANT_UNIT',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateOrgan`
(
INOUT id BIGINT,
IN typeId BIGINT,
IN siteId BIGINT,
IN manufacturerId BIGINT,
IN nuclearPlantUnitId BIGINT,
IN name VARCHAR(10),
IN serialNumber VARCHAR(10),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ORGANS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO ORGANS
        (
            ORGAN_TYPE,
            ORGAN_SITE_ID,
            ORGAN_MANUFACTURER_ID,
            ORGAN_NUCLEAR_PLANT_UNIT_ID,
            ORGAN_NAME,
            ORGAN_SERIAL_NUMBER,
            ORGAN_CREATION_DATE
        )
        VALUES
        (
            typeId,
            siteId,
            manufacturerId,
            nuclearPlantUnitId,
            name,
            serialNumber,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ORGANS', 'ORGAN',  id, date );
		
    ELSE
    
        IF NOT FcOrganExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ORGANS', id );
        END IF;

        UPDATE 
            ORGANS 
        SET
            ORGAN_TYPE = typeId,
            ORGAN_SITE_ID = siteId,
            ORGAN_MANUFACTURER_ID = manufacturerId,
            ORGAN_NUCLEAR_PLANT_UNIT_ID = nuclearPlantUnitId,
            ORGAN_NAME = name,
            ORGAN_SERIAL_NUMBER = serialNumber,
            ORGAN_MODIFICATION_DATE = NOW()
        WHERE
            ORGAN_ID = id;
            
        CALL SpGetModificationDate('ORGANS', 'ORGAN',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateRole`
(
INOUT id BIGINT,
IN name VARCHAR(100),
IN level INT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ROLES table or update an existing record'
BEGIN
    
    IF id IS NULL THEN

        INSERT INTO ROLES 
        (
            ROLE_NAME,
            ROLE_LEVEL,
            ROLE_CREATION_DATE
        )
        VALUES
        (
            name,
            level,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ROLES', 'ROLE',  id, date );

    ELSE
    
        IF NOT FcRoleExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ROLES', id );
        END IF;

        UPDATE 
            ROLES 
        SET
            ROLE_NAME = name,
            ROLE_LEVEL = level,
            ROLE_MODIFICATION_DATE = NOW()
        WHERE
            ROLE_ID = id;
            
        CALL SpGetModificationDate('ROLES', 'ROLE',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSector`
(
INOUT id BIGINT,
IN steamGeneratorTypeId BIGINT,
IN number INT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SECTORS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO SECTORS 
        (
            SECTOR_NUMBER,
            SECTOR_STEAM_GENERATOR_TYPE,
            SECTOR_CREATION_DATE
        )
        VALUES
        (
            steamGeneratorTypeId,
            number,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('SECTORS', 'SECTOR',  id, date );

    ELSE
    
        IF NOT FcSectorExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'SECTORS', id );
        END IF;

        UPDATE 
            SECTORS 
        SET
            SECTOR_NUMBER = number,
            SECTOR_STEAM_GENERATOR_TYPE = steamGeneratorTypeId,
            SECTOR_MODIFICATION_DATE = NOW()
        WHERE
            SECTOR_ID = id;
            
        CALL SpGetModificationDate('SECTORS', 'SECTOR',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSgTube`
(
IN steamGeneratorId BIGINT,
IN tubeId BIGINT,
IN hLPlugState INT,
IN cLPlugState INT,
IN serialNumber VARCHAR(10)
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SG_TUBES table or update an existing record'
BEGIN
    
	IF NOT FcSgTubeExist( tubeId, steamGeneratorId ) THEN

        INSERT INTO SG_TUBES 
        (
			SG_TUBE_STEAM_GENERATOR_ID,
            SG_TUBE_TUBE_ID,
            SG_TUBE_HOT_LEG_OBSTRUCTION_STATE,
            SG_TUBE_COLD_LEG_OBSTRUCTION_STATE,
            SG_TUBE_SERIAL_NUMBER,
            SG_TUBE_CREATION_DATE
        )
        VALUES
        (
            steamGeneratorId,
            tubeId,
            hLPlugState,
			cLPlugState,
			serialNumber,
            NOW()
        );

    ELSE

        UPDATE 
            SG_TUBES 
        SET
            SG_TUBE_STEAM_GENERATOR_ID = steamGeneratorId,
            SG_TUBE_TUBE_ID = tubeId,
            SG_TUBE_HOT_LEG_OBSTRUCTION_STATE = hLPlugState,
            SG_TUBE_COLD_LEG_OBSTRUCTION_STATE = cLPlugState,
            SG_TUBE_SERIAL_NUMBER = serialNumber,
            SG_TUBE_MODIFICATION_DATE = NOW()
        WHERE
            SG_TUBE_STEAM_GENERATOR_ID = steamGeneratorId
        AND
            SG_TUBE_TUBE_ID = tubeId;

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSite`
(
INOUT id BIGINT,
IN customerId BIGINT,
IN type SMALLINT,
IN name VARCHAR(50),
IN bigram VARCHAR(2),
IN trigram VARCHAR(3),
IN location VARCHAR(255),
IN image BLOB,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SITES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO SITES
        (
            SITE_CUSTOMER_ID,
            SITE_TYPE,
            SITE_NAME,
            SITE_BIGRAM,
            SITE_TRIGRAM,
            SITE_LOCATION,
            SITE_IMAGE,
            SITE_CREATION_DATE
        )
        VALUES
        (
            customerId,
            type,
            name,
            bigram,
            trigram,
            location,
            image,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('SITES', 'SITE',  id, date );

    ELSE
        
        IF NOT FcSiteExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'SITES', id );
        END IF;
        
        UPDATE 
            SITES 
        SET
            SITE_CUSTOMER_ID = customerId,
            SITE_TYPE = type,
            SITE_NAME = name,
            SITE_BIGRAM = bigram,
            SITE_TRIGRAM = trigram,
            SITE_LOCATION = location,
            SITE_IMAGE = image,
            SITE_MODIFICATION_DATE = NOW()
        WHERE
            SITE_ID = id;
            
        CALL SpGetModificationDate('SITES', 'SITE',  id, date );

    END IF;
	
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSteamGenerator`
(
INOUT id BIGINT,
IN siteId BIGINT,
IN manufacturerId BIGINT,
IN nuclearPlantUnitId BIGINT,
IN name VARCHAR(10),
IN serialNumber VARCHAR(10),
IN steamGeneratorTypeId BIGINT,
IN loopNumber VARCHAR(2),
IN lowRow INT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the STEAM_GENERATORS table or update an existing record'
BEGIN

    DECLARE organId BIGINT DEFAULT NULL;
	DECLARE nuclearId BIGINT DEFAULT NULL;

    IF id IS NOT NULL THEN
        SET organId =  id;
    END IF;
    
	IF nuclearPlantUnitId > 0 THEN
		SET nuclearId = nuclearPlantUnitId;
	END IF;

    CALL SpAddUpdateOrgan( organId,
                           0  ,
                           siteId,
                           manufacturerId,
                           nuclearId,
                           name,
                           serialNumber,
                           date );

    IF id IS NULL THEN

        INSERT INTO STEAM_GENERATORS
        (
            STEAM_GENERATOR_ID,
            STEAM_GENERATOR_STEAM_GENERATOR_TYPE_ID,
            STEAM_GENERATOR_LOW_ROW,
            STEAM_GENERATOR_LOOP,
            STEAM_GENERATOR_CREATION_DATE
        )
        VALUES
        (
            organId,
            steamGeneratorTypeId,
            lowRow,
            loopNumber,
            NOW()
        );

		SET id = organId;
		
    ELSE

        UPDATE 
            STEAM_GENERATORS 
        SET
            STEAM_GENERATOR_STEAM_GENERATOR_TYPE_ID = steamGeneratorTypeId,
            STEAM_GENERATOR_LOW_ROW = lowRow,
            STEAM_GENERATOR_LOOP = loopNumber,
            STEAM_GENERATOR_MODIFICATION_DATE = NOW()
        WHERE
            STEAM_GENERATOR_ID = id;

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSteamGeneratorType`
(
INOUT id BIGINT,
IN tubeMaterialName VARCHAR(30),
IN name VARCHAR(20),
IN tubeExternalDiameter DOUBLE,
IN tubeWallThickness DOUBLE,
IN columnSpacing DOUBLE,
IN rowSpacing DOUBLE,
IN angleRowAxis INT,
IN angleColumnAxis INT,
IN formula SMALLINT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the STEAM_GENERATOR_TYPES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO STEAM_GENERATOR_TYPES 
        (
            STEAM_GENERATOR_TYPE_TUBE_MATERIAL_ID,
            STEAM_GENERATOR_TYPE_NAME,
            STEAM_GENERATOR_TYPE_TUBE_EXTERNAL_DIAMETER,
            STEAM_GENERATOR_TYPE_TUBE_WALL_THICKNESS,
            STEAM_GENERATOR_TYPE_COLUMN_SPACING,
            STEAM_GENERATOR_TYPE_ROW_SPACING,
            STEAM_GENERATOR_TYPE_ANGLE_ROW_AXIS,
            STEAM_GENERATOR_TYPE_ANGLE_COLUMN_AXIS,
            STEAM_GENERATOR_TYPE_FORMULA_ID,
            STEAM_GENERATOR_TYPE_CREATION_DATE
        )
        VALUES
        (
            FcGetAttributeId( Concat('TUBE_MATERIAL_NAME.',tubeMaterialName) ),
            name,
            tubeExternalDiameter,
            tubeWallThickness,
            columnSpacing,
            rowSpacing,
            angleRowAxis,
            angleColumnAxis,
            formula,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('STEAM_GENERATOR_TYPES', 'STEAM_GENERATOR_TYPE',  id, date );

    ELSE
    
        IF NOT FcSteamGeneratorTypeExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'STEAM_GENERATOR_TYPES', id );
        END IF;

        UPDATE 
            STEAM_GENERATOR_TYPES 
        SET
            STEAM_GENERATOR_TYPE_TUBE_MATERIAL_ID = FcGetAttributeId( Concat('TUBE_MATERIAL_NAME.',tubeMaterialName) ),
            STEAM_GENERATOR_TYPE_NAME = name,
            STEAM_GENERATOR_TYPE_TUBE_EXTERNAL_DIAMETER = tubeExternalDiameter,
            STEAM_GENERATOR_TYPE_TUBE_WALL_THICKNESS = tubeWallThickness,
            STEAM_GENERATOR_TYPE_COLUMN_SPACING = columnSpacing,
            STEAM_GENERATOR_TYPE_ROW_SPACING = rowSpacing,
            STEAM_GENERATOR_TYPE_ANGLE_ROW_AXIS = angleRowAxis,
            STEAM_GENERATOR_TYPE_ANGLE_COLUMN_AXIS = angleColumnAxis,
            STEAM_GENERATOR_TYPE_FORMULA_ID = formula,
            STEAM_GENERATOR_TYPE_MODIFICATION_DATE = NOW()
        WHERE
            STEAM_GENERATOR_TYPE_ID = id;
            
        CALL SpGetModificationDate('STEAM_GENERATOR_TYPES', 'STEAM_GENERATOR_TYPE',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTask`
(
INOUT id BIGINT,
IN type SMALLINT,
IN name VARCHAR(50),
IN configurationFile VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TASKS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO TASKS 
        (
            TASK_TYPE,
            TASK_NAME,
            TASK_CONFIGURATION_FILE,
            TASK_CREATION_DATE
        )
        VALUES
        (
            type,
            name,
            configurationFile,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('TASKS', 'TASK',  id, date );

    ELSE
    
        IF NOT FcTaskExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'TASKS', id );
        END IF;

        UPDATE 
            TASKS 
        SET
            TASK_TYPE = type,
            TASK_NAME = name,
            TASK_CONFIGURATION_FILE = configurationFile,
            TASK_MODIFICATION_DATE = NOW()
        WHERE
            TASK_ID = id;
        
        CALL SpGetModificationDate('TASKS', 'TASK',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTechnique`
(
INOUT id BIGINT,
IN parentId BIGINT,
IN name VARCHAR(100),
IN version VARCHAR(5),
IN description VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TECHNIQUES table or update an existing record'
BEGIN
	
    IF parentId <= 0 THEN
        SET parentId = NULL;
    END IF;

    IF id IS NULL THEN

        INSERT INTO TECHNIQUES 
        (
            TECHNIQUE_PARENT_ID,
            TECHNIQUE_NAME,
            TECHNIQUE_VERSION,
            TECHNIQUE_DESCRIPTION,
            TECHNIQUE_CREATION_DATE
        )
        VALUES
        (
            parentId,
            name,
            version,
            description,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('TECHNIQUES', 'TECHNIQUE',  id, date );

    ELSE
    
        IF NOT FcTechniqueExist( id ) THEN
            CALL RaiseRecordNotFoundError( 'TECHNIQUES', id );
        END IF;

        UPDATE 
            TECHNIQUES 
        SET
            TECHNIQUE_PARENT_ID = parentId,
            TECHNIQUE_NAME = name,
            TECHNIQUE_VERSION = version,
            TECHNIQUE_DESCRIPTION = description,
            TECHNIQUE_MODIFICATION_DATE = NOW()
        WHERE
            TECHNIQUE_ID = id;
            
        CALL SpGetModificationDate('TECHNIQUES', 'TECHNIQUE',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTube`
(
INOUT id BIGINT,
IN sectorId BIGINT,
IN steamGeneratorTypeId BIGINT,
IN rowHotLeg INT,
IN columnHotLeg INT,
IN rowColdLeg INT,
IN columnColdLeg INT,
IN xHotLeg FLOAT,
IN yHotLeg FLOAT,
IN xColdLeg FLOAT,
IN yColdLeg FLOAT,
IN length FLOAT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TUBES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO TUBES 
        (
            TUBE_SECTOR_ID,
            TUBE_STEAM_GENERATOR_TYPE_ID,
            TUBE_ROW_HOT_LEG,
            TUBE_COLUMN_HOT_LEG,
            TUBE_ROW_COLD_LEG,
            TUBE_COLUMN_COLD_LEG,
            TUBE_X_HOT_LEG,
            TUBE_Y_HOT_LEG,
            TUBE_X_COLD_LEG,
            TUBE_Y_COLD_LEG,
            TUBE_LENGTH,
            TUBE_CREATION_DATE
        )
        VALUES
        (
            sectorId,
            steamGeneratorTypeId,
            rowHotLeg,
            columnHotleg,
            rowColdLeg,
            columnColdLeg,
            xHotLeg,
            yHotLeg,
            xColdLeg,
            yColdLeg,
            length,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('TUBES', 'TUBE',  id, date );

    ELSE
    
        IF NOT FcTubeExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'TUBES', id );
        END IF;

        UPDATE 
            TUBES 
        SET
            TUBE_SECTOR_ID = sectorId,
            TUBE_STEAM_GENERATOR_TYPE_ID = steamGeneratorTypeId,
            TUBE_ROW_HOT_LEG = rowHotLeg,
            TUBE_COLUMN_HOT_LEG = columnHotLeg,
            TUBE_ROW_COLD_LEG = rowColdLeg,
            TUBE_COLUMN_COLD_LEG = columnColdLeg,
            TUBE_X_HOT_LEG = xHotLeg,
            TUBE_Y_HOT_LEG = yHotLeg,
            TUBE_X_COLD_LEG = xColdLeg,
            TUBE_Y_COLD_LEG = yColdLeg,
            TUBE_LENGTH = length,
            TUBE_MODIFICATION_DATE = NOW()
        WHERE
            TUBE_ID = id;
            
        CALL SpGetModificationDate('TUBES', 'TUBE',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateUser`
(
INOUT id BIGINT,
IN certificationLevel INT,
IN login VARCHAR(100),
IN hashPassword VARCHAR(255),
IN lastName VARCHAR(100),
IN firstName VARCHAR(100),
IN company VARCHAR(100),
IN image BLOB,
IN isRoot TINYINT(1),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the USERS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO USERS 
        (
            USER_CERTIFICATION_LEVEL,
            USER_LOGIN,
            USER_HASH_PASSWORD,
            USER_LAST_NAME,
            USER_FIRST_NAME,
            USER_COMPANY,
            USER_IMAGE,
            USER_IS_ROOT,
            USER_CREATION_DATE
        )
        VALUES
        (
            certificationLevel,
            login,
            hashPassword,
            lastName,
            firstName,
            company, 
            image,
            isRoot,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('USERS', 'USER',  id, date );

    ELSE
    
        IF NOT FcUserExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'USERS', id );
        END IF;

        UPDATE 
            USERS 
        SET
            USER_CERTIFICATION_LEVEL = certificationLevel,
            USER_LOGIN = login,
            USER_HASH_PASSWORD = hashPassword,
            USER_FIRST_NAME = firstName,
            USER_LAST_NAME = lastName,
            USER_COMPANY = company,
            USER_IMAGE = image,
            USER_IS_ROOT = isRoot,
            USER_MODIFICATION_DATE = NOW()
        WHERE
            USER_ID = id;
            
        CALL SpGetModificationDate('USERS', 'USER',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateZone`
(
INOUT id BIGINT,
IN steamGeneratorTypeId BIGINT,
IN name VARCHAR(30),
IN color INT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ZONES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO ZONES 
        (
            ZONE_STEAM_GENERATOR_TYPE_ID,
            ZONE_NAME,
            ZONE_COLOR,
            ZONE_CREATION_DATE
        )
        VALUES
        (
            steamGeneratorTypeId,
            name,
            color,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ZONES', 'ZONE',  id, date );

    ELSE
    
        IF NOT FcZoneExist( id ) THEN
            CALL RaiseRecordNotFoundError( 'ZONES', id );
        END IF;

        UPDATE 
            ZONES 
        SET
            ZONE_STEAM_GENERATOR_TYPE_ID = steamGeneratorTypeId,
            ZONE_NAME = name,
            ZONE_COLOR = color,
            ZONE_MODIFICATION_DATE = NOW()
        WHERE
            ZONE_ID = id;
            
        CALL SpGetModificationDate('ZONES', 'ZONE',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateExam`
(
INOUT id BIGINT,
IN controlId BIGINT,
IN tubeId BIGINT,
IN beginZoneId BIGINT,
IN endZoneId BIGINT,
IN direction INT,
IN beginOffset INT,
IN endOffset INT,
IN translationSpeed FLOAT,
IN rotationSpeed FLOAT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EXAMS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO EXAMS 
        (
            EXAM_CONTROL_ID,
			EXAM_TUBE_ID,
            EXAM_BEGIN_ZONE_ID,
            EXAM_END_ZONE_ID,
            EXAM_DIRECTION,
            EXAM_BEGIN_OFFSET,
            EXAM_END_OFFSET,
	    EXAM_TRANSLATION_SPEED,
	    EXAM_ROTATION_SPEED,
            EXAM_CREATION_DATE
        )
        VALUES
        (
            controlId,
			tubeId,
            beginZoneId,
            endZoneId,
            direction,
            beginOffset,
            endOffset,
	    translationSpeed,
	    rotationSpeed,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('EXAMS', 'EXAM',  id, date );

    ELSE
    
        IF NOT FcExamExist( id ) THEN
            CALL SpRaiseRecordNotFound( 'EXAMS', id );
        END IF;

        UPDATE 
            EXAMS 
        SET
            EXAM_CONTROL_ID = controlId,
            EXAM_TUBE_ID = tubeId,
			EXAM_BEGIN_ZONE_ID = beginZoneId,
            EXAM_END_ZONE_ID = endZoneId,
            EXAM_DIRECTION = direction,
            EXAM_BEGIN_OFFSET = beginOffset,
            EXAM_END_OFFSET = endOffset,
	    EXAM_TRANSLATION_SPEED = translationSpeed,
	    EXAM_ROTATION_SPEED = rotationSpeed,
            EXAM_MODIFICATION_DATE = NOW()
        WHERE
            EXAM_ID = id;
            
        CALL SpGetModificationDate('EXAMS', 'EXAM',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateComponent`
(
INOUT id BIGINT,
IN parentId BIGINT,
IN type Smallint,
IN name VARCHAR(255),
IN description VARCHAR(255),
IN connectionString VARCHAR(255),
IN version VARCHAR(255),
IN code VARCHAR(10),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the COMPONENTS table or update an existing record'
BEGIN

    IF parentId <= 0 THEN
	    SET parentId = NULL;
	END IF;

    IF id IS NULL THEN

        INSERT INTO COMPONENTS 
        (
            COMPONENT_TYPE,
            COMPONENT_PARENT_ID,
            COMPONENT_NAME,
            COMPONENT_DESCRIPTION,
            COMPONENT_SERVER_CONNECTION_STRING,
            COMPONENT_VERSION,
            COMPONENT_CODE,
            COMPONENT_CREATION_DATE
        )
        VALUES
        (
            type,
            parentId,
            name,
            description,
			connectionString,
            version,
            code,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('COMPONENTS', 'COMPONENT',  id, date );

    ELSE
    
        IF NOT FcComponentExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'COMPONENTS', id );
        END IF;

        UPDATE 
            COMPONENTS 
        SET
            COMPONENT_TYPE = type,
            COMPONENT_PARENT_ID = parentId,
            COMPONENT_NAME = name,
            COMPONENT_DESCRIPTION = description,
			COMPONENT_SERVER_CONNECTION_STRING = connectionString,
            COMPONENT_VERSION = version,
			COMPONENT_CODE = code,
            COMPONENT_MODIFICATION_DATE = NOW()
        WHERE
            COMPONENT_ID = id;
            
        CALL SpGetModificationDate('COMPONENTS', 'COMPONENT',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateConversion`
(
IN unitSourceId BIGINT,
IN unitDestId BIGINT,
IN converterType SMALLINT,
IN parameters VARCHAR(255)
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the CONVERSIONS table or update an existing record'
BEGIN
    
    IF FcConversionExist( unitSourceId, unitDestId ) = FALSE THEN
    
        INSERT INTO CONVERSIONS
        (
            CONVERSION_UNIT_SOURCE_ID,
            CONVERSION_UNIT_DEST_ID,
            CONVERSION_CONVERTER_TYPE,
            CONVERSION_PARAMETERS,
            CONVERSION_CREATION_DATE
        )
        VALUES
        (
            unitSourceId,
            unitDestId,
            converterId,
            parameters,
            NOW()
        );
        
    ELSE
    
        UPDATE
            CONVERSIONS
        SET
            CONVERSION_UNIT_SOURCE_ID = unitSourceId,
            CONVERSION_UNIT_DEST_ID = unitDestId,
            CONVERSION_CONVERTER_TYPE = converterType,
            CONVERSION_PARAMETERS = parameters,
            CONVERSION_MODIFICATION_DATE = NOW()
        WHERE   
            CONVERSION_UNIT_SOURCE_ID = unitSourceId
        AND
            CONVERSION_UNIT_DEST_ID = unitDestId;
    
    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSelectedEquipment`
(
IN controlId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to remove the records from the SELECTED_EQUIPMENTS table corresponding to the specified control identifier'
BEGIN

    START TRANSACTION;

    DELETE FROM
        SELECTED_EQUIPMENTS
    WHERE
        SELECTED_EQUIPMENT_CONTROL_ID = controlId;
        
    COMMIT;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSelectedEquipments`
(
IN controlId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to retrieve the selected equipments for the given control in the SELECTED_EQUIPMENTS table'
BEGIN

    SELECT
        SE.SELECTED_EQUIPMENT_EQUIPMENT_ID AS EQUIPMENT_ID,
        E.EQUIPMENT_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
        E.EQUIPMENT_STATUS AS STATUS,
        E.EQUIPMENT_SERIAL_NUMBER AS SERIAL_NUMBER,
        E.EQUIPMENT_NAME AS NAME,
        SE.SELECTED_EQUIPMENT_ACQUISITION_LINE_ID AS ACQUISITION_LINE_ID
    FROM
        SELECTED_EQUIPMENTS SE
            JOIN EQUIPMENTS E ON SE.SELECTED_EQUIPMENT_EQUIPMENT_ID = E.EQUIPMENT_ID
			JOIN ACQUISITION_LINES AL ON AL.ACQUISITION_LINE_ID = SE.SELECTED_EQUIPMENT_ACQUISITION_LINE_ID
    WHERE
        SE.SELECTED_EQUIPMENT_CONTROL_ID = controlId
    ORDER BY
        AL.ACQUISITION_LINE_NUMBER ASC;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpLockTable`
(
IN tableName VARCHAR(255)
)
SQL SECURITY DEFINER
COMMENT 'Allows to lock a table'
BEGIN

    DECLARE result INT DEFAULT NULL;

    WHILE result != 1 DO
    
        SELECT 
            GET_LOCK( tableName , 10 )
        INTO
            result;
            
    END WHILE;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpReleaseLock`
(
IN tableName VARCHAR(255)
)
SQL SECURITY DEFINER
COMMENT 'Release a lock on a table'
BEGIN

    SELECT RELEASE_LOCK( tableName );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpSelectEquipment`
(
IN equipmentId BIGINT,
IN controlId BIGINT,
IN acquisitionLineId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SELECTED_EQUIPMENTS table'
BEGIN

    START TRANSACTION;

    INSERT INTO SELECTED_EQUIPMENTS
    (
        SELECTED_EQUIPMENT_CONTROL_ID,
        SELECTED_EQUIPMENT_EQUIPMENT_ID,
        SELECTED_EQUIPMENT_ACQUISITION_LINE_ID
    )
    VALUES
    (
        equipmentId,
        controlId,
        acquisitionLineId
    );
	
	COMMIT;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpUnselectEquipment`
(
IN equipmentId BIGINT,
IN controlId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to remove a record from the SELECTED_EQUIPMENTS table corresponding to the specified equipment identifier and control identifier'
BEGIN

    DELETE FROM
        SELECTED_EQUIPMENTS
    WHERE
        SELECTED_EQUIPMENT_CONTROL_ID = controlId
    AND
        SELECTED_EQUIPMENT_EQUIPMENT_ID = equipmentId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddAcquisitionEquipment`
(
IN acquisitionId BIGINT,
IN equipmentId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACQUISITIONS_EQUIPMENTS table'
BEGIN

    INSERT INTO ACQUISITIONS_EQUIPMENTS
    (
        ACQUISITION_EQUIPMENT_ACQUISITION_ID,
        ACQUISITION_EQUIPMENT_EQUIPMENT_ID,
        ACQUISITION_EQUIPMENT_CREATION_DATE
    )
    VALUES
    (
        acquisitionId,
        equipmentId,
        NOW()
    );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddApplicationFunctionalityRole`
(
IN roleId BIGINT,
IN applicationFunctionalityId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the APPLICATION_FUNCTIONALITIES_ROLES table'
BEGIN

    INSERT INTO APPLICATION_FUNCTIONALITIES_ROLES
    (
        AFR_APPLICATION_FUNCTIONALITY_ID,
        AFR_ROLE_ID,
        AFR_CREATION_DATE
    )
    VALUES
    (
        applicationFunctionalityId,
        roleId,
        NOW()
    );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipment`
(
INOUT id BIGINT,
IN equipmentTypeId BIGINT,
IN status INT,
IN eqCondition INT,
IN fmeStatus INT,
IN serialNumber VARCHAR(30),
IN name VARCHAR(30),
IN connectionString VARCHAR(255),
IN moduleConnectionString VARCHAR(255),
IN version VARCHAR(50),
IN isTemplate TINYINT(1),
IN customAttributesLayoutFile VARCHAR(255),
IN comment TEXT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENTS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO EQUIPMENTS 
        (
            EQUIPMENT_EQUIPMENT_TYPE_ID,
            EQUIPMENT_STATE,
            EQUIPMENT_CONDITION,
            EQUIPMENT_FME_STATUS,
            EQUIPMENT_SERIAL_NUMBER,
            EQUIPMENT_NAME,
            EQUIPMENT_CONNECTION_STRING,
            EQUIPMENT_MODULE_CONNECTION_STRING,
            EQUIPMENT_VERSION,
            EQUIPMENT_IS_TEMPLATE,
            EQUIPMENT_CUSTOM_ATTRIBUTES_LAYOUT_FILE,
            EQUIPMENT_COMMENT,
            EQUIPMENT_CREATION_DATE
        )
        VALUES
        (
            equipmentTypeId,
            status,
            eqCondition,
            fmeStatus,
            serialNumber,
            name,
            connectionString,
            moduleConnectionString,
            version,
            isTemplate,
            customAttributesLayoutFile,
            comment,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('EQUIPMENTS', 'EQUIPMENT',  id, date );

    ELSE
    
        IF NOT FcEquipmentExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'EQUIPMENTS', id );
        END IF;

        UPDATE 
            EQUIPMENTS 
        SET
            EQUIPMENT_EQUIPMENT_TYPE_ID = equipmentTypeID,
            EQUIPMENT_STATE = status,
            EQUIPMENT_CONDITION = eqCondition,
            EQUIPMENT_FME_STATUS = fmeStatus,
            EQUIPMENT_SERIAL_NUMBER = serialNumber,
            EQUIPMENT_NAME = name,
            EQUIPMENT_CONNECTION_STRING = connectionString,
            EQUIPMENT_MODULE_CONNECTION_STRING = moduleConnectionString,
            EQUIPMENT_VERSION = version,
            EQUIPMENT_IS_TEMPLATE = isTemplate,
            EQUIPMENT_CUSTOM_ATTRIBUTES_LAYOUT_FILE = customAttributesLayoutFile,
            EQUIPMENT_COMMENT = comment,
            EQUIPMENT_MODIFICATION_DATE = NOW()
        WHERE
            EQUIPMENT_ID = id;
            
        CALL SpGetModificationDate('EQUIPMENTS', 'EQUIPMENT',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionFilesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACQUISITION_FILES table'
BEGIN

    CALL SpDeleteAcquisitionFiles( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionGroupsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACQUISITION_GROUPS table'
BEGIN

    CALL SpDeleteAcquisitionGroups( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionsEquipmentsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACQUISITIONS_EQUIPMENTS table'
BEGIN

    CALL SpDeleteAcquisitionsEquipments( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACQUISITIONS table'
BEGIN

    CALL SpDeleteAcquisitions( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : ACQUISITION_GROUPS, ACQUISITIONS, ACQUISITION_FILES, QUALITY_CHECK_RESULTS, QUALITY_CHECK_RESULT_VALUES, ACQUISITION_GROUPS_LINES'
BEGIN

    CALL SpClearQualityCheckResultValuesTable();
    CALL SpClearQualityCheckResultsTable();
    CALL SpClearAcquisitionsEquipmentsTable();
    CALL SpClearAcquisitionGroupsLinesTable();
    CALL SpClearAcquisitionFilesTable();
    CALL SpClearAcquisitionsTable();
    CALL SpClearAcquisitionGroupsTable();

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearApplicationFunctionalitiesRolesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the APPLICATION_FUNCTIONALITIES_ROLES table'
BEGIN

    CALL SpDeleteApplicationFunctionalitiesRoles( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearApplicationFunctionalitiesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the APPLICATION_FUNCTIONALITIES table'
BEGIN

    CALL SpDeleteApplicationFunctionalities( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAttributesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttributes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearBackupHistoriesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the BACKUP_HISTORIES table'
BEGIN

    CALL SpDeleteBackupHistories( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearComponentsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the COMPONENTS table'
BEGIN

    CALL SpDeleteComponents( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearControlAttributesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONTROL_ATTRIBUTES table'
BEGIN

    CALL SpDeleteControlAttributes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearControlsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONTROLS table'
BEGIN

    CALL SpDeleteControls( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearControlZonesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONTROL_ZONES table'
BEGIN

    CALL SpDeleteControlZones( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearControlZonesTubesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONTROL_ZONES_TUBES table'
BEGIN

    CALL SpDeleteControlZonesTubes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearConversionsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONVERSIONS table'
BEGIN

    CALL SpDeleteConversions( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearDatabase`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears all tables'
BEGIN

    CALL SpClearMiscellaneousWorspaceTables();
    CALL SpClearFileTransfertWorspaceTables();
    CALL SpClearEventWorspaceTables();
    CALL SpClearAcquisitionWorspaceTables();
    CALL SpClearInspectionWorspaceTables();
    CALL SpClearTechniqueWorspaceTables();
    CALL SpClearOrganWorspaceTables();
    CALL SpClearSteamGeneratorTypeWorspaceTables();
    CALL SpClearEquipmentWorspaceTables();
    CALL SpClearEquipmentTypeWorspaceTables();
    CALL SpClearSecurityWorspaceTables();
    CALL SpClearLocalizationWorspaceTables();
    CALL SpClearEAVWorspaceTables();
    CALL SpClearUnitWorspaceTables();
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEAVWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : ATTRIBUTES'
BEGIN

    CALL SpClearAttributesTable();
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentConfigurationsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EQUIPMENT_CONFIGURATIONS table'
BEGIN

    CALL SpDeleteFiles( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentsAttributesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EQUIPMENTS_ATTRIBUTES table'
BEGIN

    CALL SpDeleteEquipmentsAttributes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EQUIPMENTS table'
BEGIN

    CALL SpDeleteEquipments( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentTrackingsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EQUIPMENT_TRACKINGS table'
BEGIN

    CALL SpDeleteEquipmentTrackings( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentTypeAttributesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EQUIPMENT_TYPES_ATTRIBUTES table'
BEGIN

    CALL SpDeleteEquipmentTypeAttributes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentTypeWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : EQUIPMENT_TYPES, EQUIPMENT_TYPES_ATTRIBUTES, IOS'
BEGIN

    CALL SpClearEquipmentTypesTable();
    
    
    

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEquipmentWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : EQUIPMENTS, EQUIPMENTS_ATTRIBUTES, EQUIPMENT_TRACKINGS, EQUIPMENT_TRACKINGS_ATTRIBUTES'
BEGIN

    CALL SpClearEquipmentsTable();
    
    
    
    

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEventsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EVENTS table'
BEGIN

    CALL SpDeleteEvents( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearEventWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : EVENTS'
BEGIN

    CALL SpClearEventsTable();

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearExamsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the EXAMS table'
BEGIN

    CALL SpDeleteExams( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearFileTransferWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : QUEUE_FILES_TRANSFER'
BEGIN

    CALL SpClearQueueFilesTransfer();

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearGeometricZonesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the GEOMETRIC_ZONES table'
BEGIN

    CALL SpDeleteGeometricZones( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearInspectionsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the INSPECTIONS table'
BEGIN

    CALL SpDeleteInspections( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearInspectionWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : INSPECTIONS, CONTROLS, CHARACTERISTIC_TUBES, CONTROL_ZONES, CONTROL_ZONES_TUBES, EXAMS, SELECTED_CONTROLS, SELECTED_EQUIPMENTS'
BEGIN

    CALL SpClearSelectedControlsTable();
    CALL SpClearSelectedEquipmentsTable();
    CALL SpClearExamsTable();
    CALL SpClearControlsTable();
    CALL SpClearInpsectionsTable();

    
    
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearLanguagesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the LANGUAGES table'
BEGIN

    CALL SpDeleteLanguages( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearLayoutsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the LAYOUTS table'
BEGIN

    CALL SpDeleteLayouts( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearLocalizationWorkspaceTables`
()
SQL SECURITY DEFINER
COMMENT 'Clear the tables : TEXTS, LANGUAGES'
BEGIN

    CALL SpClearTextsTable();
    CALL SpClearLanguagesTable();
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearManwaysTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the MANWAYS table'
BEGIN

    CALL SpDeleteManways( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearMiscellaneousWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : BACKUP_HISTORIES, TASKS, LAYOUTS, SUPERVISOR_CLIENTS'
BEGIN

    CALL SpClearBakupHistoriesTable();
    CALL SpClearTasksTable();
    CALL SpClearLayousTable();
    CALL SpClearSupervisorClientsTable();
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearNuclearPlantUnitsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the NUCLEAR_PLANT_UNITS table'
BEGIN

    CALL SpDeleteNuclearPlantUnits( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearOrgansTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ORGANS table'
BEGIN

    CALL SpDeleteOrgans( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearOrganWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : SITES, NUCLEAR_PLANT_UNITS, ORGANS, STEAM_GENERATORS, MANWAYS, SG_TUBES, SG_TUBES_GEOMETRIC_ZONES'
BEGIN

    CALL SpClearSteamGeneratorsTable();
    CALL SpClearOrgansTable();
    CALL SpClearNuclearPlantUnitsTable();
    CALL SpClearSitesTable();
    
    
    
    
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearRolesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ROLES table'
BEGIN

    CALL SpDeleteRoles( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSectorsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the SECTORS table'
BEGIN

    CALL SpDeleteSectors( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSecurityWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : USERS, ROLES, APPLICATION_FUNCTIONALITIES, ROLES_USERS, APPLICATION_FUNCTIONALITIES_ROLES'
BEGIN

    CALL SpClearUsersTable();
    CALL SpClearRolesTable();
    CALL SpClearApplicationFunctionalitiesTable();
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSgTubesGeometricZonesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the SG_TUBES_GEOMETRIC_ZONES table'
BEGIN

    CALL SpDeleteSgTubesGeometricZones( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSgTubesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the SG_TUBES table'
BEGIN

    CALL SpDeleteSgTubes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSitesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the SITES table'
BEGIN

    CALL SpDeleteSites( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSteamGeneratorsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the STEAM_GENERATORS table'
BEGIN

    CALL SpDeleteSteamGenerators( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSteamGeneratorTypesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the STEAM_GENERATOR_TYPES table'
BEGIN

    CALL SpDeleteSteamGeneratorTypes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSteamGeneratorTypeWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : STEAM_GENERATOR_TYPES, GEOMETRIC_ZONES, SECTORS, ZONES, TUBES, TUBES_ZONES, TUBES_GEOMETRIC_ZONES'
BEGIN

    CALL SpClearSteamGeneratorTypes();
    
    
    
    
    
    
    
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearTasksTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the TASKS table'
BEGIN

    CALL SpDeleteTasks( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearTechniquesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the TECHNIQUES table'
BEGIN

    CALL SpDeleteTechniques( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearTechniqueWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : TECHNIQUES, EQUIPMENT_TYPES_TECHNIQUES, EQUIPMENT_CONFIGURATIONS, EQUIPMENT_TYPES_CONFIGURATIONS'
BEGIN

    CALL SpClearEquipmentConfigurationsTable();
    CALL SpClearTechniquesTable();
    CALL SpClearEquipmentConfigurationsTable();
    
    
    
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearTextsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the TEXTS table'
BEGIN

    CALL SpDeleteTexts( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearTubesGeometricZonesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the TUBES_GEOMETRIC_ZONES table'
BEGIN

    CALL SpDeleteTubesGeometricZones( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearTubesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the TUBES table'
BEGIN

    CALL SpDeleteTubes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearUnitsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the UNITS table'
BEGIN

    CALL SpDeleteUnits( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearUnitWorkspaceTables`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the tables : UNITS, CONVERTERS, CONVERSIONS, UNIT_CONFIGURATIONS, UNITS_IN_UNIT_CONFIGURATIONS'
BEGIN

    CALL SpClearConversionsTable();
    CALL SpClearConverterTable();
    CALL SpClearUnitsTable();
    
    
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearUsersTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the USERS table'
BEGIN

    CALL SpDeleteUsers( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisition`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACQUISITIONS table'
BEGIN

    DELETE FROM 
        ACQUISITIONS
    WHERE
        ACQUISITION_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionEquipment`
(
IN acquisitionId BIGINT,
IN equipmentId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACQUISITIONS_EQUIPMENTS table'
BEGIN

    DELETE FROM 
        ACQUISITIONS_EQUIPMENTS
    WHERE
        ACQUISITION_EQUIPMENT_ACQUISITION_ID = acquisitionId
    AND
        ACQUISITION_EQUIPMENT_EQUIPMENT_ID = equipmentId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionFile`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACQUISITION_FILES table'
BEGIN

    DELETE FROM 
        ACQUISITION_FILES
    WHERE
        ACQUISITION_FILE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionFiles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACQUISITION_FILES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACQUISITION_FILES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionGroup`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACQUISITION_GROUPS table'
BEGIN

    DELETE FROM 
        ACQUISITION_GROUPS
    WHERE
        ACQUISITION_GROUP_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionGroups`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACQUISITION_GROUPS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACQUISITION_GROUPS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitions`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACQUISITIONS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACQUISITIONS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionsEquipments`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACQUISITIONS_EQUIPMENTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACQUISITIONS_EQUIPMENTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteApplicationFunctionalities`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the APPLICATION_FUNCTIONALITIES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'APPLICATION_FUNCTIONALITIES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteApplicationFunctionalitiesRoles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the APPLICATION_FUNCTIONALITIES_ROLES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'APPLICATION_FUNCTIONALITIES_ROLES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteApplicationFunctionality`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the APPLICATION_FUNCTIONALITIES table'
BEGIN

    DELETE FROM 
        APPLICATION_FUNCTIONALITIES
    WHERE
        APPLICATION_FUNCTIONALITY_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteApplicationFunctionalityRole`
(
IN roleId BIGINT,
IN applicationFunctionalityId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the APPLICATION_FUNCTIONALITIES_ROLES table'
BEGIN

    DELETE FROM 
        APPLICATION_FUNCTIONALITIES_ROLES
    WHERE
        AFR_APPLICATION_FUNCTIONALITY_ID = applicationFunctionalityId
    AND
        AFR_ROLE_ID = roleId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAttributes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ATTRIBUTES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ATTRIBUTES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteBackupHistories`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the BACKUP_HISTORIES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'BACKUP_HISTORIES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteComponent`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the COMPONENTS table'
BEGIN

    DELETE FROM 
        COMPONENTS
    WHERE
        COMPONENT_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteComponents`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the COMPONENTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'COMPONENTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentConfigurations`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENT_CONFIGURATIONS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENT_CONFIGURATIONS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteRecords`
(
IN tableName VARCHAR(50),
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Delete records from the specified table matching with the whereClause'
BEGIN

    SET @strQuery = CONCAT( 'DELETE FROM ', tableName , ' ', whereClause );

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;                    

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSgTubes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the SG_TUBES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'SG_TUBES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSite`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SITES table'
BEGIN

    DELETE FROM
        SITES
    WHERE
        SITE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSites`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the SITES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'SITES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSteamGenerator`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the STEAM_GENERATORS table'
BEGIN

    DELETE FROM
        STEAM_GENERATORS
    WHERE
        STEAM_GENERATOR_ID = id;

	CALL SpDeleteOrgan (id);
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSteamGenerators`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the STEAM_GENERATORS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'STEAM_GENERATORS', whereClause );
	CALL SpDeleteOrgans ( whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSteamGeneratorType`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the STEAM_GENERATOR_TYPES table'
BEGIN

    DELETE FROM
        STEAM_GENERATOR_TYPES
    WHERE
        STEAM_GENERATOR_TYPE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSteamGeneratorTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the STEAM_GENERATOR_TYPES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'STEAM_GENERATOR_TYPES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTask`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the TASKS table'
BEGIN

    DELETE FROM
        TASKS
    WHERE
        TASK_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTasks`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TASKS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'TASKS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTechnique`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the TECHNIQUES table'
BEGIN

    DELETE FROM
        TECHNIQUES
    WHERE
        TECHNIQUE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTechniques`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TECHNIQUES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'TECHNIQUES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTexts`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TEXTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'TEXTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTube`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the TUBES table'
BEGIN

    DELETE FROM
        TUBES
    WHERE
        TUBE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTubesGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TUBES_GEOMETRIC_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'TUBES_GEOMETRIC_ZONES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUnit`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the UNITS table'
BEGIN

    DELETE FROM
        UNITS
    WHERE
        UNIT_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUnits`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the UNITS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'UNITS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUser`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the USERS table'
BEGIN

    DELETE FROM
        USERS
    WHERE
        USER_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUsers`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the USERS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'USERS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteZone`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ZONES table'
BEGIN

    DELETE FROM
        ZONES
    WHERE
        ZONE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionFiles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITION_FILES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            AF.ACQUISITION_FILE_ID AS ID,
                            AF.ACQUISITION_FILE_ACQUISITION_ID AS ACQUISITION_ID,
                            AF.ACQUISITION_FILE_TYPE AS FILE_TYPE,
                            AF.ACQUISITION_FILE_NAME AS NAME,
                            AF.ACQUISITION_FILE_TRANSFER_DATE AS TRANSFER_DATE,
                            AF.ACQUISITION_FILE_CREATION_DATE AS CREATION_DATE,
                            AF.ACQUISITION_FILE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ACQUISITION_FILES AF'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionGroups`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITION_GROUPS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            AG.ACQUISITION_GROUP_ID AS ID,
                            AG.ACQUISITION_GROUP_CONTROL_ID AS CONTROL_ID,
                            AG.ACQUISITION_GROUP_STATE AS STATE,
                            AG.ACQUISITION_GROUP_NUMBER AS NUMBER,
                            AG.ACQUISITION_GROUP_NAME AS NAME,
                            AG.ACQUISITION_GROUP_DIRECTORY AS DIRECTORY,
                            AG.ACQUISITION_GROUP_CREATION_DATE AS CREATION_DATE,
                            AG.ACQUISITION_GROUP_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ACQUISITION_GROUPS AG'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitions`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITIONS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            A.ACQUISITION_ID AS ID,
                            A.ACQUISITION_ACQUISITION_GROUP_ID AS ACQUISITION_GROUP_ID,
                            A.ACQUISITION_EXAM_ID AS EXAM_ID,
                            A.ACQUISITION_PARENT_ID AS PARENT_ID,
                            A.ACQUISITION_CHILD_ID AS CHILD_ID,
                            A.ACQUISITION_CYCLE_STATE AS CYCLE_STATE,
                            A.ACQUISITION_TYPE AS TYPE,
                            A.ACQUISITION_POSITION_OK AS POSITION_OK,
                            A.ACQUISITION_QUALITY_CHECK_STATE AS QUALITY_CHECK_STATE,
                            A.ACQUISITION_CYCLE_NAME AS CYCLE_NAME,
                            A.ACQUISITION_IS_AUTOMATIC_MODE AS IS_AUTOMATIC_MODE,
                            A.ACQUISITION_BEGIN_DATE AS BEGIN_DATE,
                            A.ACQUISITION_FINISH_DATE AS FINISH_DATE,
                            A.ACQUISITION_RESUME_REASON AS RESUME_REASON,
                            A.ACQUISITION_RESUME_CODE AS RESUME_CODE,
                            A.ACQUISITION_RESUME_TRANSMITTER AS RESUME_TRANSMITTER,
                            A.ACQUISITION_CREATION_DATE AS CREATION_DATE,
                            A.ACQUISITION_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ACQUISITIONS A'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionsEquipments`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITIONS_EQUIPMENTS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            AE.ACQUISITION_EQUIPMENT_EQUIPMENT_ID AS EQUIPMENT_ID,
                            AE.ACQUISITION_EQUIPMENT_ACQUISITION_ID AS ACQUISITION_ID,
                            E.EQUIPMENT_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
                            ET.EQUIPMENT_TYPE_NAME AS EQUIPMENT_TYPE_NAME,
                            E.EQUIPMENT_NAME AS EQUIPMENT_NAME,
                            E.EQUIPMENT_SERIAL_NUMBER AS EQUIPMENT_SERIAL_NUMBER
                        FROM
                            ACQUISITIONS_EQUIPMENTS AE
                                JOIN EQUIPMENTS E ON AE.ACQUISITION_EQUIPMENT_EQUIPMENT_ID = E.EQUIPMENT_ID
                                JOIN EQUIPMENT_TYPES ET ON E.EQUIPMENT_EQUIPMENT_TYPE_ID = ET.EQUIPMENT_TYPE_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetApplicationFunctionalities`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the APPLICATION_FUNCTIONALITIES_ROLES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            AF.APPLICATION_FUNCTIONALITY_ID AS ID,
                            AF.APPLICATION_FUNCTIONALITY_CODE AS CODE,
                            AF.APPLICATION_FUNCTIONALITY_DESCRIPTION AS DESCRIPTION,
                            AF.APPLICATION_FUNCTIONALITY_CREATION_DATE AS CREATION_DATE,
                            AF.APPLICATION_FUNCTIONALITY_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            APPLICATION_FUNCTIONALITIES AF'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetComponents`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the COMPONENTS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            C.COMPONENT_ID AS ID,
                            C.COMPONENT_TYPE AS TYPE,
                            C.COMPONENT_PARENT_ID AS PARENT_ID,
                            C.COMPONENT_NAME AS NAME,
                            C.COMPONENT_DESCRIPTION AS DESCRIPTION,
                            C.COMPONENT_VERSION AS VERSION,
							C.COMPONENT_CODE AS CODE,
							C.COMPONENT_SERVER_CONNECTION_STRING AS SERVER_CONNECTION_STRING,
                            C.COMPONENT_CREATION_DATE AS CREATION_DATE,
                            C.COMPONENT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            COMPONENTS C'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentConfigurations`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENT_CONFIGURATIONS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            EC.EQUIPMENT_CONFIGURATION_ID AS ID,
                            EC.EQUIPMENT_CONFIGURATION_CONTROL_ID AS CONTROL_ID,
                            EC.EQUIPMENT_CONFIGURATION_LINE_NUMBER AS LINE_NUMBER,
                            F.FILE_TYPE AS TYPE,
			    F.FILE_NAME AS NAME,
                            F.FILE_PATH AS PATH,
			    F.FILE_VERSION AS VERSION,
                            EC.EQUIPMENT_CONFIGURATION_CONFIGURATION_NAME AS CONFIGURATION_NAME,
                            EC.EQUIPMENT_CONFIGURATION_DESCRIPTION AS DESCRIPTION,
                            EC.EQUIPMENT_CONFIGURATION_CREATION_DATE AS CREATION_DATE,
                            EC.EQUIPMENT_CONFIGURATION_MODIFICATION_DATE AS MODIFICATION_DATE                        
                        FROM
                            EQUIPMENT_CONFIGURATIONS EC
                                JOIN FILES F ON EC.EQUIPMENT_CONFIGURATION_ID = F.FILE_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetRecords`
(
IN selectQuery VARCHAR(5000),
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

    SET @strQuery = CONCAT( selectQuery, ' ', whereClause );

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSectors`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SECTORS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            S.SECTOR_ID AS ID,
                            S.SECTOR_STEAM_GENERATOR_TYPE_ID AS STEAM_GENERATOR_TYPE_ID,
                            S.SECTOR_NUMBER AS NUMBER,
                            S.SECTOR_CREATION_DATE AS CREATION_DATE,
                            S.SECTOR_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            SECTORS S '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSgTubes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SG_TUBES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            SGT.SG_TUBE_STEAM_GENERATOR_ID AS STEAM_GENERATOR_ID,
                            SGT.SG_TUBE_TUBE_ID AS TUBE_ID,
                            O.ORGAN_NAME AS STEAM_GENERATOR_NAME,
                            T.TUBE_ROW_HOT_LEG AS ROW_HOT_LEG,
                            T.TUBE_COLUMN_HOT_LEG AS COLUMN_HOT_LEG,
                            T.TUBE_ROW_COLD_LEG AS ROW_COLD_LEG,
                            T.TUBE_COLUMN_COLD_LEG AS COLUMN_COLD_LEG,
                            SGT.SG_TUBE_HOT_LEG_OBSTRUCTION_STATE AS HOT_LEG_OBSTRUCTION_STATE,
                            SGT.SG_TUBE_COLD_LEG_OBSTRUCTION_STATE AS COLD_LEG_OBSTRUCTION_STATE,
                            SGT.SG_TUBE_SERIAL_NUMBER AS SERIAL_NUMBER,
                            SGT.SG_TUBE_CREATION_DATE AS CREATION_DATE,
                            SGT.SG_TUBE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            SG_TUBES SGT
                                INNER JOIN TUBES T ON T.TUBE_ID = SGT.SG_TUBE_TUBE_ID
                                INNER JOIN ORGANS O ON O.ORGAN_ID = SG_TUBE_STEAM_GENERATOR_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSites`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SITES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            S.SITE_ID AS ID,
                            S.SITE_TYPE AS TYPE,
                            S.SITE_CUSTOMER_ID AS CUSTOMER_ID,
                            S.SITE_NAME AS NAME,
                            S.SITE_BIGRAM AS BIGRAM,
                            S.SITE_TRIGRAM AS TRIGRAM,
                            S.SITE_LOCATION AS LOCATION,
                            S.SITE_IMAGE AS IMAGE,
                            S.SITE_CREATION_DATE AS CREATION_DATE,
                            S.SITE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            SITES S '
                    ,whereClause );
                    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSteamGenerators`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the STEAM_GENERATORS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            SG.STEAM_GENERATOR_ID AS ID,
                            SG.STEAM_GENERATOR_STEAM_GENERATOR_TYPE_ID AS STEAM_GENERATOR_TYPE_ID,
                            O.ORGAN_SITE_ID AS SITE_ID,
                            O.ORGAN_MANUFACTURER_ID AS MANUFACTURER_ID,
                            O.ORGAN_NUCLEAR_PLANT_UNIT_ID AS NUCLEAR_PLANT_UNIT_ID,
                            O.ORGAN_NAME AS NAME,
                            O.ORGAN_SERIAL_NUMBER AS SERIAL_NUMBER,
                            SG.STEAM_GENERATOR_LOW_ROW AS LOW_ROW,
                            SG.STEAM_GENERATOR_LOOP AS LOOP_NUMBER,
                            SG.STEAM_GENERATOR_CREATION_DATE AS CREATION_DATE,
                            SG.STEAM_GENERATOR_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            STEAM_GENERATORS SG
                                JOIN ORGANS O ON SG.STEAM_GENERATOR_ID = O.ORGAN_ID '
                    ,whereClause );
                    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSteamGeneratorTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the STEAM_GENERATOR_TYPES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            SGT.STEAM_GENERATOR_TYPE_ID AS ID,
                            SGT.STEAM_GENERATOR_TYPE_TUBE_MATERIAL_ID AS TUBE_MATERIAL_ID,
                            SGT.STEAM_GENERATOR_TYPE_NAME AS NAME,
                            SGT.STEAM_GENERATOR_TYPE_TUBE_EXTERNAL_DIAMETER AS TUBE_EXTERNAL_DIAMETER,
                            SGT.STEAM_GENERATOR_TYPE_TUBE_WALL_THICKNESS AS TUBE_WALL_THICKNESS,
                            TB.ATTRIBUTE_NAME AS TUBE_MATERIAL_NAME,
                            SGT.STEAM_GENERATOR_TYPE_ROW_SPACING AS ROW_SPACING,
                            SGT.STEAM_GENERATOR_TYPE_COLUMN_SPACING AS COLUMN_SPACING,
                            SGT.STEAM_GENERATOR_TYPE_ANGLE_ROW_AXIS AS ANGLE_ROW_AXIS,
                            SGT.STEAM_GENERATOR_TYPE_ANGLE_COLUMN_AXIS AS ANGLE_COLUMN_AXIS,
                            SGT.STEAM_GENERATOR_TYPE_FORMULA_ID AS FORMULA_ID,
                            SGT.STEAM_GENERATOR_TYPE_CREATION_DATE AS CREATION_DATE,
                            SGT.STEAM_GENERATOR_TYPE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            STEAM_GENERATOR_TYPES SGT
                                LEFT JOIN ATTRIBUTES TB ON SGT.STEAM_GENERATOR_TYPE_TUBE_MATERIAL_ID = TB.ATTRIBUTE_ID '
                    ,whereClause );
                    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTechniques`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TECHNIQUES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            T.TECHNIQUE_ID AS ID,
                            T.TECHNIQUE_PARENT_ID AS PARENT_ID,
                            T.TECHNIQUE_NAME AS NAME,
                            T.TECHNIQUE_VERSION AS VERSION,
                            T.TECHNIQUE_DESCRIPTION AS DESCRIPTION,
                            T.TECHNIQUE_CREATION_DATE AS CREATION_DATE,
                            T.TECHNIQUE_MODIFICATION_DATE AS MODIFICATION_DATE 
                        FROM
                            TECHNIQUES T '
                    ,whereClause );
                    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTubes`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all record present in the TUBES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            T.TUBE_ID AS ID,
                            T.TUBE_STEAM_GENERATOR_TYPE_ID AS STEAM_GENERATOR_TYPE_ID,
                            T.TUBE_SECTOR_ID AS SECTOR_ID,
                            SGT.STEAM_GENERATOR_TYPE_NAME AS STEAM_GENERATOR_TYPE_NAME,
                            S.SECTOR_NUMBER AS SECTOR_NUMBER,
                            T.TUBE_ROW_HOT_LEG AS ROW_HOT_LEG,
                            T.TUBE_COLUMN_HOT_LEG AS COLUMN_HOT_LEG,
                            T.TUBE_ROW_COLD_LEG AS ROW_COLD_LEG,
                            T.TUBE_COLUMN_COLD_LEG AS COLUMN_COLD_LEG,
                            T.TUBE_X_HOT_LEG AS X_HOT_LEG,
                            T.TUBE_Y_HOT_LEG AS Y_HOT_LEG,
                            T.TUBE_X_COLD_LEG AS X_COLD_LEG,
                            T.TUBE_Y_COLD_LEG AS Y_COLD_LEG,      
                            T.TUBE_LENGTH AS LENGTH,
                            T.TUBE_CREATION_DATE AS CREATION_DATE,
                            T.TUBE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            TUBES T 
                                JOIN STEAM_GENERATOR_TYPES SGT ON T.TUBE_STEAM_GENERATOR_TYPE_ID = SGT.STEAM_GENERATOR_TYPE_ID
                                LEFT JOIN SECTORS S ON T.TUBE_SECTOR_ID = S.SECTOR_ID '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetZones`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all record present in the ZONES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            Z.ZONE_ID AS ID,
                            Z.ZONE_STEAM_GENERATOR_TYPE_ID AS STEAM_GENERATOR_TYPE_ID,
                            SGT.STEAM_GENERATOR_TYPE_NAME AS STEAM_GENERATOR_TYPE_NAME,
                            Z.ZONE_NAME AS NAME,
                            Z.ZONE_COLOR AS COLOR,
                            Z.ZONE_CREATION_DATE as CREATION_DATE,
                            Z.ZONE_MODIFICATION_DATE as MODIFICATION_DATE
                        FROM
                            ZONES Z 
                                JOIN STEAM_GENERATOR_TYPES SGT ON Z.ZONE_STEAM_GENERATOR_TYPE_ID = SGT.STEAM_GENERATOR_TYPE_ID '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpRaiseError`
(
IN errorCode BIGINT,
IN message VARCHAR(256))
SQL SECURITY DEFINER
BEGIN
	SIGNAL SQLSTATE 'ERROR'
    
    SET
        MESSAGE_TEXT = message,
        MYSQL_ERRNO = errorCode;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpRaiseRecordNotFoundError`
(
IN tableName VARCHAR(100),
IN id BIGINT)
SQL SECURITY DEFINER
BEGIN
	
    CALL SpRaiseError( 1000, CONCAT( 'Record with Id [', id, '] not found in table ', tableName ) );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddControlZoneTube`
(
IN controlZoneId BIGINT,
IN tubeId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the CONTROL_ZONES_TUBES table'
BEGIN

    INSERT INTO CONTROL_ZONES_TUBES 
    (
        CONTROL_ZONE_TUBE_ZONE_ID,
        CONTROL_ZONE_TUBE_TUBE_ID,
        CONTROL_ZONE_TUBE_CREATION_DATE
    )
    VALUES
    (
        controlZoneId,
        tubeId,
        NOW()
    );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateControlAttribute`
(
IN controlId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the CONTROLS_ATTRIBUTES table or update an existing record'
BEGIN

	SET date = NOW();

    IF NOT FcControlAttributeExist( controlId, attributeId ) THEN

        INSERT INTO CONTROLS_ATTRIBUTES 
        (
            CONTROL_ATTRIBUTE_CONTROL_ID,
            CONTROL_ATTRIBUTE_ATTRIBUTE_ID,
            CONTROL_ATTRIBUTE_VALUE,
            CONTROL_ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            controlId,
            attributeId,
            value,
			date
        );

    ELSE

        UPDATE 
            CONTROLS_ATTRIBUTES 
        SET
            CONTROL_ATTRIBUTE_VALUE = value,
            CONTROL_ATTRIBUTE_MODIFICATION_DATE = date
        WHERE
            CONTROL_ATTRIBUTE_CONTROL_ID = controlId
        AND
            CONTROL_ATTRIBUTE_ATTRIBUTE_ID = attributeId;

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateControlZone`
(
INOUT id BIGINT,
IN controlId BIGINT,
IN techniqueId BIGINT,
IN name VARCHAR(100),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the CONTROL_ZONES table or update an existing record'
BEGIN

    IF id IS NULL THEN
    
        INSERT INTO CONTROL_ZONES 
        (
            CONTROL_ZONE_CONTROL_ID,
            CONTROL_ZONE_TECHNIQUE_ID,
            CONTROL_ZONE_NAME,
            CONTROL_ZONE_CREATION_DATE
        )
        VALUES
        (
            controlId,
            techniqueId,
            name,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('CONTROL_ZONES', 'CONTROL_ZONE',  id, date );
        
    ELSE
    
        IF NOT FcControlZoneExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'CONTROL_ZONES', id );
        END IF;

        UPDATE 
            CONTROL_ZONES 
        SET
            CONTROL_ZONE_CONTROL_ID = controlId,
            CONTROL_ZONE_TECHNIQUE_ID = techniqueId,
            CONTROL_ZONE_NAME = name,
            CONTROL_ZONE_MODIFICATION_DATE = NOW()
        WHERE
            CONTROL_ZONE_ID = id;
            
        CALL SpGetModificationDate('CONTROL_ZONES', 'CONTROL_ZONE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentAttribute`
(
IN equipmentId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENTS_ATTRIBUTES table or update an existing record'
BEGIN

    IF FcEquipmentAttributeExist( equipmentId, attributeId ) = FALSE THEN
  
        INSERT INTO EQUIPMENTS_ATTRIBUTES 
        (
            EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID,
            EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID,
            EQUIPMENT_ATTRIBUTE_VALUE,
            EQUIPMENT_ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            equipmentId,
            attributeId,
            value,
            NOW()
        );
             CALL SpGetCreationDateFromLinkedTable('EQUIPMENTS_ATTRIBUTES', 'EQUIPMENT_ATTRIBUTE',  CONCAT( ' WHERE EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID = ', equipmentId, ' AND EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID = ', attributeId), date );
    ELSE
    
        UPDATE
            EQUIPMENTS_ATTRIBUTES 
        SET
            EQUIPMENT_ATTRIBUTE_VALUE = value,
            EQUIPMENT_ATTRIBUTE_MODIFICATION_DATE = NOW()
        WHERE
            EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID = equipmentId
        AND
            EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID = attributeId;

        CALL SpGetModificationDateFromLinkedTable('EQUIPMENTS_ATTRIBUTES', 'EQUIPMENT_ATTRIBUTE',  CONCAT( ' WHERE EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID = ', equipmentId, ' AND EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID = ', attributeId), date );
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentConfiguration`
(
INOUT id BIGINT,
IN lineNumber SMALLINT,
IN controlId BIGINT,
IN type SMALLINT,
IN name VARCHAR(255),
IN fileName VARCHAR(255),
IN filePath VARCHAR(255),
IN description VARCHAR(255),
IN version VARCHAR(10),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENT_CONFIGURATIONS table or update an existing record'
BEGIN

    DECLARE fileId BIGINT DEFAULT NULL;
    
    SET date = NOW();
    
    IF id IS NOT NULL THEN
        SET fileId =  id;
    END IF;
    
    CALL SpAddUpdateFile( fileId, fileName, type, filePath, version , date );

    IF id IS NULL THEN
    
        INSERT INTO EQUIPMENT_CONFIGURATIONS 
        (
            EQUIPMENT_CONFIGURATION_ID,
            EQUIPMENT_CONFIGURATION_CONTROL_ID,
            EQUIPMENT_CONFIGURATION_LINE_NUMBER,
            EQUIPMENT_CONFIGURATION_CONFIGURATION_NAME,
            EQUIPMENT_CONFIGURATION_DESCRIPTION,
            EQUIPMENT_CONFIGURATION_CREATION_DATE
        )
        VALUES
        (
            fileId,
            controlId,
            lineNumber,
            name,
            description,
            date
        );
        
    ELSE

        UPDATE 
            EQUIPMENT_CONFIGURATIONS 
        SET
            EQUIPMENT_CONFIGURATION_CONTROL_ID = controlId,
            EQUIPMENT_CONFIGURATION_LINE_NUMBER = lineNumber,
            EQUIPMENT_CONFIGURATION_CONFIGURATION_NAME = name,
            EQUIPMENT_CONFIGURATION_DESCRIPTION = description,
            EQUIPMENT_CONFIGURATION_MODIFICATION_DATE = date
        WHERE
            EQUIPMENT_CONFIGURATION_ID = id;

    END IF;

	SET id = fileId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentTracking`
(
INOUT id BIGINT,
IN equipmentId BIGINT,
IN controlId BIGINT,
IN userId BIGINT,
IN fmeStatus INT,
IN eqCondition INT,
IN installDate DATETIME,
IN removalDate DATETIME,
IN lineNumber INT,
IN removalComment Longtext,
IN missingParts TEXT,
IN folderName VARCHAR(255),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENT_TRACKINGS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO EQUIPMENT_TRACKINGS 
        (
            EQUIPMENT_TRACKING_EQUIPMENT_ID,
            EQUIPMENT_TRACKING_CONTROL_ID,
            EQUIPMENT_TRACKING_USER_ID,
            EQUIPMENT_TRACKING_FME_STATUS,
            EQUIPMENT_TRACKING_CONDITION,
            EQUIPMENT_TRACKING_INSTALL_DATE,
            EQUIPMENT_TRACKING_REMOVAL_DATE,
            EQUIPMENT_TRACKING_LINE_NUMBER,
            EQUIPMENT_TRACKING_REMOVAL_COMMENT,
            EQUIPMENT_TRACKING_MISSING_PARTS,
            EQUIPMENT_TRACKING_FOLDER_NAME,
            EQUIPMENT_TRACKING_CREATION_DATE
        )
        VALUES
        (
            equipmentId,
            controlId,
            userId,
            fmeStatus,
            eqCondition,
            installDate,
            removalDate,
            lineNumber,
            removalComment,
            missingParts,
            folderName,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('EQUIPMENT_TRACKINGS', 'EQUIPMENT_TRACKING',  id, date );

    ELSE
    
        IF NOT FcEquipmentTrackingExist( id ) THEN
            CALL SpRaiseRecordNotFound( 'EQUIPMENT_TRACKINGS', id );
        END IF;

        UPDATE 
            EQUIPMENT_TRACKINGS 
        SET
            EQUIPMENT_TRACKING_EQUIPMENT_ID = equipmentId,
            EQUIPMENT_TRACKING_CONTROL_ID = controlId,
            EQUIPMENT_TRACKING_USER_ID = userId,
            EQUIPMENT_TRACKING_FME_STATUS = fmeStatus,
            EQUIPMENT_TRACKING_CONDITION = eqCondition,
            EQUIPMENT_TRACKING_INSTALL_DATE = installDate,
            EQUIPMENT_TRACKING_REMOVAL_DATE = removalDate,
            EQUIPMENT_TRACKING_LINE_NUMBER = lineNumber,
            EQUIPMENT_TRACKING_REMOVAL_COMMENT = removalComment,
            EQUIPMENT_TRACKING_MISSING_PARTS = missingParts,
            EQUIPMENT_TRACKING_FOLDER_NAME = folderName,
            EQUIPMENT_TRACKING_MODIFICATION_DATE = NOW()
        WHERE
            EQUIPMENT_TRACKING_ID = id;
            
        CALL SpGetModificationDate('EQUIPMENT_TRACKINGS', 'EQUIPMENT_TRACKING',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentType`
(
INOUT id BIGINT,
IN manufacturerId BIGINT,
IN categoryId BIGINT,
IN autoDetectCategoryId BIGINT,
IN componentId BIGINT,
IN name VARCHAR(20),
IN image BLOB,
IN partNumber VARCHAR(30),
IN description VARCHAR(255),
IN isAcquisitionLineEquipment TINYINT(1),
IN isTrackable TINYINT(1),
IN isTemplate TINYINT(1),
IN allowMultipleSelection TINYINT(1),
IN customAttributesLayoutFile VARCHAR(255),
IN equipmentConnectionStringFormat VARCHAR(255),
IN moduleConnectionStringFormat VARCHAR(255),
IN fileExtension VARCHAR(10),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENT_TYPES table or update an existing record'
BEGIN
    
	IF autoDetectCategoryId = 0 THEN
		SET autoDetectCategoryId = NULL;
	END IF;

    IF id IS NULL THEN
    
        INSERT INTO EQUIPMENT_TYPES
        (
            EQUIPMENT_TYPE_MANUFACTURER_ID,
            EQUIPMENT_TYPE_CATEGORY_ID,
            EQUIPMENT_TYPE_COMPONENT_ID,
            EQUIPMENT_TYPE_AUTO_DETECT_CATEGORY_ID,
            EQUIPMENT_TYPE_NAME,
            EQUIPMENT_TYPE_IMAGE,
            EQUIPMENT_TYPE_PART_NUMBER,
            EQUIPMENT_TYPE_DESCRIPTION,
            EQUIPMENT_TYPE_IS_ACQUISITION_LINE_EQUIPMENT,
            EQUIPMENT_TYPE_IS_TRACKABLE,
            EQUIPMENT_TYPE_IS_TEMPLATE,
            EQUIPMENT_TYPE_ALLOW_MULTIPLE_SELECTION,
            EQUIPMENT_TYPE_CUSTOM_ATTRIBUTES_LAYOUT_FILE,
            EQUIPMENT_TYPE_EQUIPMENT_CONNECTION_STRING_FORMAT,
            EQUIPMENT_TYPE_MODULE_CONNECTION_STRING_FORMAT,
            EQUIPMENT_TYPE_FILE_EXTENSION,
            EQUIPMENT_TYPE_CREATION_DATE
        )
        VALUES
        (
            manufacturerId,
            categoryId,
            componentId,
            autoDetectCategoryId,
            name,
            image,
            partNumber,
            description,
            isAcquisitionLineEquipment,
            isTrackable,
            isTemplate,
            allowMultipleSelection,
            customAttributesLayoutFile,
            equipmentConnectionStringFormat,
            moduleConnectionStringFormat,    
            fileExtension,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('EQUIPMENT_TYPES', 'EQUIPMENT_TYPE',  id, date );
        
    ELSE
    
        IF NOT FcEquipmentTypeExist( id ) THEN
            CALL SpRaiseRecordNotFound( 'EQUIPMENT_TYPES', id );
        END IF;

        UPDATE
            EQUIPMENT_TYPES
        SET
            EQUIPMENT_TYPE_MANUFACTURER_ID = manufacturerId,
            EQUIPMENT_TYPE_CATEGORY_ID = categoryId,
            EQUIPMENT_TYPE_COMPONENT_ID = componentId,
            EQUIPMENT_TYPE_AUTO_DETECT_CATEGORY_ID = autoDetectCategoryId,
            EQUIPMENT_TYPE_NAME = name,
            EQUIPMENT_TYPE_IMAGE = image,
            EQUIPMENT_TYPE_PART_NUMBER = partNumber,
            EQUIPMENT_TYPE_DESCRIPTION = description,
            EQUIPMENT_TYPE_IS_ACQUISITION_LINE_EQUIPMENT = isAcquisitionLineEquipment,
            EQUIPMENT_TYPE_IS_TRACKABLE = istrackable,
            EQUIPMENT_TYPE_IS_TEMPLATE = isTemplate,
            EQUIPMENT_TYPE_ALLOW_MULTIPLE_SELECTION = allowMultipleSelection,
            EQUIPMENT_TYPE_CUSTOM_ATTRIBUTES_LAYOUT_FILE = customAttributesLayoutFile,
            EQUIPMENT_TYPE_EQUIPMENT_CONNECTION_STRING_FORMAT = equipmentConnectionStringFormat,
            EQUIPMENT_TYPE_MODULE_CONNECTION_STRING_FORMAT = moduleConnectionStringFormat,  
            EQUIPMENT_TYPE_FILE_EXTENSION = fileExtension, 
            EQUIPMENT_TYPE_MODIFICATION_DATE = NOW()
        WHERE   
            EQUIPMENT_TYPE_ID = id;
            
        CALL SpGetModificationDate('EQUIPMENT_TYPES', 'EQUIPMENT_TYPE',  id, date );
    
    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateFile`
(
INOUT id BIGINT,
IN name VARCHAR(255),
IN type INT,
IN path VARCHAR(255),
IN version VARCHAR(10),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the FILES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO FILES 
        (
			FILE_NAME,
            FILE_TYPE,
            FILE_PATH,     
            FILE_VERSION,
            FILE_CREATION_DATE
        )
        VALUES
        (
			name,
            type,
            path,
            version,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('FILES', 'FILE',  id, date );

    ELSE
    
        IF NOT FcFileExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'FILES', id );
        END IF;

        UPDATE 
            FILES 
        SET
			FILE_NAME = name,
            FILE_TYPE = type,
            FILE_PATH = path,
            FILE_VERSION = version,
            FILE_MODIFICATION_DATE = NOW()
        WHERE
            FILE_ID = id;
            
        CALL SpGetModificationDate('FILES', 'FILE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSgTubeGeometricZone`
(
IN tubeGeometricZoneId BIGINT,
IN steamGeneratorId BIGINT,
IN altitude FLOAT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SG_TUBES_GEOMETRIC_ZONES table or update an existing record'
BEGIN

    IF NOT FcSgTubeGeometricZoneExist( tubeGeometricZoneId, steamGeneratorId ) THEN

        INSERT INTO SG_TUBES_GEOMETRIC_ZONES 
        (
            SG_TUBES_GEOMETRIC_ZONE_ID,
            SG_TUBES_GEOMETRIC_ZONE_STEAM_GENERATOR_ID,
            SG_TUBES_GEOMETRIC_ZONE_ALTITUDE,
            SG_TUBES_GEOMETRIC_ZONE_CREATION_DATE
        )
        VALUES
        (
            tubeGeometricZoneId,
            steamGeneratorId,
            altitude,
            NOW()
        );

    ELSE

        UPDATE 
            SG_TUBES_GEOMETRIC_ZONES 
        SET
            SG_TUBES_GEOMETRIC_ZONE_ID = tubeGeometricZoneId,
            SG_TUBES_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId,
            SG_TUBES_GEOMETRIC_ZONE_ALTITUDE = altitude,
            SG_TUBES_GEOMETRIC_ZONE_MODIFICATION_DATE = NOW()
        WHERE
            SG_TUBES_GEOMETRIC_ZONE_ID = tubeGeometricZoneId
        AND
            SG_TUBES_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId;

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUserRole`
(
IN userId BIGINT,
IN roleId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the USERS_ROLES table'
BEGIN

    INSERT INTO USERS_ROLES
    (
        USER_ID,
        ROLE_ID 
    )
    VALUES
    (
        userId,
        roleId
    );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearControlsAttributesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONTROLS_ATTRIBUTES table'
BEGIN

    CALL SpDeleteControlsAttributes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearFilesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the FILES table'
BEGIN

    CALL SpDeleteFiles( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlAttribute`
(
IN controlId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the CONTROLS_ATTRIBUTES table'
BEGIN

    DELETE FROM
        CONTROLS_ATTRIBUTES
    WHERE
        CONTROL_ATTRIBUTE_CONTROL_ID = controlId
	AND
		CONTROL_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControls`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the CONTROLS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'CONTROLS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlsAttributes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the CONTROLS_ATTRIBUTES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'CONTROLS_ATTRIBUTES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlZone`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the CONTROL_ZONES table'
BEGIN

    DELETE FROM
        CONTROL_ZONES
    WHERE
        CONTROL_ZONE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the CONTROL_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'CONTROL_ZONES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlZonesTubes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the CONTROL_ZONES_TUBES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'CONTROL_ZONES_TUBES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlZoneTube`
(
IN controlZoneId BIGINT,
IN tubeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the CONTROL_ZONES_TUBES table'
BEGIN

    DELETE FROM
        CONTROL_ZONES_TUBES
    WHERE
        CONTROL_ZONE_TUBE_ZONE_ID = controlZoneId
    AND
        CONTROL_ZONE_TUBE_TUBE_ID = tubeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteConversion`
(
IN unitSourceId BIGINT,
IN unitDestId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the CONVERSIONS table'
BEGIN

    DELETE FROM
        CONVERSIONS
    WHERE
        CONVERSION_UNIT_SOURCE_ID = unitSourceId
    AND
        CONVERSION_UNIT_DEST_ID = unitDestId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteConversions`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the CONVERSIONS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'CONVERSIONS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipment`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EQUIPMENTS table'
BEGIN

    DELETE FROM
        EQUIPMENTS
    WHERE
        EQUIPMENT_ID = id;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentConfiguration`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EQUIPMENT_CONFIGURATIONS table'
BEGIN

    CALL SpDeleteFile( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipments`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentsAttributes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENTS_ATTRIBUTES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENTS_ATTRIBUTES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentTracking`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EQUIPMENT_TRACKINGS table'
BEGIN

    DELETE FROM
        EQUIPMENT_TRACKINGS
    WHERE
        EQUIPMENT_TRACKING_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentTrackings`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENT_TRACKINGS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENT_TRACKINGS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentType`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EQUIPMENT_TYPES table'
BEGIN

    DELETE FROM
        EQUIPMENT_TYPES
    WHERE
        EQUIPMENT_TYPE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentTypeAttribute`
(
IN equipmentTypeId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EQUIPMENT_TYPES_ATTRIBUTES table'
BEGIN

    DELETE FROM
        EQUIPMENT_TYPES_ATTRIBUTES
    WHERE
        ETA_EQUIPMENT_TYPE_ID = equipmentTypeId
    AND
        ETA_ATTRIBUTE_ID = attributeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENT_TYPES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENT_TYPES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentTypesAttributes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENT_TYPES_ATTRIBUTES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENT_TYPES_ATTRIBUTES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEvent`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EVENTS table'
BEGIN

    DELETE FROM
        EVENTS
    WHERE
        EVENT_ID = id;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEvents`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EVENTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EVENTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteExam`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EXAM table'
BEGIN

    DELETE FROM 
        EXAMS
    WHERE
        EXAM_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteExams`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EXAMS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EXAMS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteFile`
(
IN id INT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the FILES table'
BEGIN

    DELETE FROM
        FILES
    WHERE
        FILE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteFiles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the FILES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'FILES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteGeometricZone`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the GEOMETRIC_ZONES table'
BEGIN

    DELETE FROM 
        GEOMETRIC_ZONES
    WHERE
        GEOMETRIC_ZONE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the GEOMETRIC_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'GEOMETRIC_ZONES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteInspection`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the INSPECTIONS table'
BEGIN

    DELETE FROM 
        INSPECTIONS
    WHERE
        INSPECTION_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteInspections`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the INSPECTIONS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'INSPECTIONS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteLanguages`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the LANGUAGES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'LANGUAGES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteLayout`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the LAYOUTS table'
BEGIN

    DELETE FROM 
        LAYOUTS
    WHERE
        LAYOUT_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteLayouts`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the LAYOUTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'LAYOUTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteManway`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the MANWAYS table'
BEGIN

    DELETE FROM 
        MANWAYS
    WHERE
        MANWAY_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteManways`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the MANWAYS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'MANWAYS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteNuclearPlantUnit`
(
IN id INT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the NUCLEAR_PLANT_UNITS table'
BEGIN

    DELETE FROM 
        NUCLEAR_PLANT_UNITS
    WHERE
        NUCLEAR_PLANT_UNIT_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteNuclearPlantUnits`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the NUCLEAR_PLANT_UNITS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'NUCLEAR_PLANT_UNITS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteOrgan`
(
IN id INT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ORGANS table'
BEGIN

    DELETE FROM 
        ORGANS
    WHERE
        ORGAN_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteOrgans`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ORGANS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ORGANS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteRole`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ROLES table'
BEGIN

    DELETE FROM 
        ROLES
    WHERE
        ROLE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteRoles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ROLES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ROLES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSector`
(
IN id INT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SECTORS table'
BEGIN

    DELETE FROM 
        SECTORS
    WHERE
        SECTOR_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSectors`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the SECTORS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'SECTORS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSgTube`
(
IN steamGeneratorId BIGINT,
IN tubeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SG_TUBES table'
BEGIN

    DELETE FROM 
        SG_TUBES
    WHERE
        SG_TUBE_STEAM_GENERATOR_ID = steamGeneratorId
    AND
        SG_TUBE_TUBE_ID = tubeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSgTubeGeometricZone`
(
IN tubeGeometricZoneId BIGINT,
IN steamGeneratorId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SG_TUBES_GEOMETRIC_ZONES table'
BEGIN

    DELETE FROM 
        SG_TUBES_GEOMETRIC_ZONES
    WHERE
        SG_TUBE_GEOMETRIC_ZONE_ID = tubeGeometricZoneId
    AND
        SG_TUBE_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSgTubesGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the SG_TUBES_GEOMETRIC_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'SG_TUBES_GEOMETRIC_ZONES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUserRole`
(
IN userId INT,
IN roleId INT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the USERS_ROLES table'
BEGIN

    DELETE FROM 
        USERS_ROLES
    WHERE
        USER_ID = userId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUsersRoles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the USERS_ROLES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'USERS_ROLES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROL_ZONES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            CZ.CONTROL_ZONE_ID AS ID,
                            CZ.CONTROL_ZONE_CONTROL_ID AS CONTROL_ID,
                            CZ.CONTROL_ZONE_TECHNIQUE_ID AS TECHNIQUE_ID,
                            CZ.CONTROL_ZONE_NAME AS NAME,
                            CZ.CONTROL_ZONE_CREATION_DATE AS CREATION_DATE,
                            CZ.CONTROL_ZONE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            CONTROL_ZONES CZ'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlZonesTubes`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROL_ZONES_TUBES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            CZT.CONTROL_ZONE_TUBE_ZONE_ID as CONTROL_ZONE_ID,
                            CZT.CONTROL_ZONE_TUBE_TUBE_ID as TUBE_ID,
                            CZ.CONTROL_ZONE_CONTROL_ID AS CONTROL_ID,
                            C.CONTROL_NAME AS CONTROL_NAME,
                            CZ.CONTROL_ZONE_NAME AS CONTROL_ZONE_NAME,
                            T.TUBE_ROW AS TUBE_ROW_HL,
                            T.TUBE_COLUMN AS TUBE_COLUMN_HL,
                            T.TUBE_ROW_CL AS TUBE_ROW_CL,
                            T.TUBE_COLUMN_CL AS TUBE_COLUMN_CL,
                            CZT.CONTROL_ZONE_TUBE_CREATION_DATE AS CREATION_DATE
                        FROM
                            CONTROL_ZONES_TUBES CZT
                                INNER JOIN TUBES T ON CZT.CONTROL_ZONE_TUBE_TUBE_ID = T.TUBE_ID
                                INNER JOIN CONTROL_ZONES CZ ON CZT.CONTROL_ZONE_TUBE_ZONE_ID = CZ.CONTROL_ZONE_ID
                                INNER JOIN CONTROLS C ON CZ.CONTROL_ZONE_CONTROL_ID = C.CONTROL_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentTrackings`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENT_TRACKINGS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            ET.EQUIPMENT_TRACKING_ID AS ID,
                            ET.EQUIPMENT_TRACKING_EQUIPMENT_ID AS EQUIPMENT_ID,
                            ET.EQUIPMENT_TRACKING_CONTROL_ID AS CONTROL_ID,
                            ET.EQUIPMENT_TRACKING_USER_ID AS USER_ID,
                            ET.EQUIPMENT_TRACKING_FME_STATUS AS FME_STATUS,
                            ET.EQUIPMENT_TRACKING_CONDITION AS ECONDITION,
                            ET.EQUIPMENT_TRACKING_INSTALL_DATE AS INSTALL_DATE,
                            ET.EQUIPMENT_TRACKING_REMOVAL_DATE AS REMOVAL_DATE,
                            ET.EQUIPMENT_TRACKING_LINE_NUMBER AS LINE_NUMBER,
                            ET.EQUIPMENT_TRACKING_REMOVAL_COMMENT AS REMOVAL_COMMENT,
                            ET.EQUIPMENT_TRACKING_FOLDER_NAME AS FOLDER_NAME,
                            ET.EQUIPMENT_TRACKING_MISSING_PARTS AS MISSING_PARTS,
                            ET.EQUIPMENT_TRACKING_CREATION_DATE AS CREATION_DATE,
                            ET.EQUIPMENT_TRACKING_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            EQUIPMENT_TRACKINGS ET'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentTypes`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENT_TYPES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            ET.EQUIPMENT_TYPE_ID AS ID,
                            ET.EQUIPMENT_TYPE_MANUFACTURER_ID AS MANUFACTURER_ID,
                            ET.EQUIPMENT_TYPE_CATEGORY_ID AS CATEGORY_ID,
                            ET.EQUIPMENT_TYPE_AUTO_DETECT_CATEGORY_ID AS AUTO_DETECT_CATEGORY_ID,
                            ET.EQUIPMENT_TYPE_COMPONENT_ID AS COMPONENT_ID,
                            ET.EQUIPMENT_TYPE_NAME AS NAME,
                            ET.EQUIPMENT_TYPE_IMAGE AS IMAGE,
                            ET.EQUIPMENT_TYPE_PART_NUMBER AS PART_NUMBER,
                            ET.EQUIPMENT_TYPE_DESCRIPTION AS DESCRIPTION,
                            ET.EQUIPMENT_TYPE_IS_ACQUISITION_LINE_EQUIPMENT AS IS_ACQUISITION_LINE_EQUIPMENT,
                            ET.EQUIPMENT_TYPE_IS_TRACKABLE AS IS_TRACKABLE,
                            ET.EQUIPMENT_TYPE_IS_TEMPLATE AS IS_TEMPLATE,
                            ET.EQUIPMENT_TYPE_ALLOW_MULTIPLE_SELECTION AS ALLOW_MULTIPLE_SELECTION,
                            ET.EQUIPMENT_TYPE_CUSTOM_ATTRIBUTES_LAYOUT_FILE AS CUSTOM_ATTRIBUTES_LAYOUT_FILE,
                            ET.EQUIPMENT_TYPE_EQUIPMENT_CONNECTION_STRING_FORMAT AS EQUIPMENT_CONNECTION_STRING_FORMAT,
                            ET.EQUIPMENT_TYPE_MODULE_CONNECTION_STRING_FORMAT AS MODULE_CONNECTION_STRING_FORMAT,    
                            ET.EQUIPMENT_TYPE_FILE_EXTENSION AS FILE_EXTENSION,
                            ET.EQUIPMENT_TYPE_CREATION_DATE AS CREATION_DATE,
                            ET.EQUIPMENT_TYPE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            EQUIPMENT_TYPES ET'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEvents`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EVENTS table matching the where clause.'
BEGIN
    
    DROP TABLE IF EXISTS EVENTS_TOTAL;
    
    CREATE TABLE EVENTS_TOTAL(
    `EVENT_ID` Bigint UNSIGNED NOT NULL AUTO_INCREMENT,
    `EVENT_CONTROL_ID` Bigint UNSIGNED,
    `EVENT_ACQUISITION_GROUP_ID` Bigint UNSIGNED,
    `EVENT_TYPE` Smallint NOT NULL,
    `EVENT_LEVEL` Smallint NOT NULL,
    `EVENT_LINE_NUMBER` Int,
    `EVENT_CODE` Varchar(20) NOT NULL,
    `EVENT_SOURCE` Varchar(100) NOT NULL,
    `EVENT_DESCRIPTION` Text NOT NULL,
    `EVENT_DETAIL1` Text,
    `EVENT_DETAIL2` Text,
    `EVENT_ACQUIT_DATE` Datetime,
    `EVENT_CREATION_DATE` Datetime NOT NULL,
    `EVENT_MODIFICATION_DATE` Datetime,
    INDEX(EVENT_ID))
    ENGINE=MRG_MyISAM
    UNION=(EVENTS,EVENTS_HISTORY)
    CHARACTER SET utf8
    INSERT_METHOD=LAST;

    CALL SpGetRecords( 'SELECT
                            E.EVENT_ID AS ID,
                            E.EVENT_CONTROL_ID AS CONTROL_ID,
                            E.EVENT_ACQUISITION_GROUP_ID AS ACQUISITION_GROUP_ID,
                            E.EVENT_LEVEL AS LEVEL,
                            E.EVENT_TYPE AS TYPE,
                            E.EVENT_LINE_NUMBER AS LINE_NUMBER,
                            E.EVENT_CODE AS CODE,
                            E.EVENT_SOURCE AS SOURCE,
                            E.EVENT_DESCRIPTION AS DESCRIPTION,
                            E.EVENT_ACQUIT_DATE AS ACQUIT_DATE,
                            E.EVENT_DETAIL1 AS DETAIL1,
                            E.EVENT_DETAIL2 AS DETAIL2,
                            E.EVENT_CREATION_DATE AS CREATION_DATE,
                            E.EVENT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            EVENTS_TOTAL E'
                    ,whereClause );                       
                                     
    DROP TABLE IF EXISTS EVENTS_TOTAL;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetExams`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EXAMS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            EXAM_ID AS ID,
                            EXAM_CONTROL_ID AS CONTROL_ID,
							EXAM_TUBE_ID AS TUBE_ID,
                            EXAM_BEGIN_ZONE_ID AS BEGIN_ZONE_ID,
                            EXAM_END_ZONE_ID AS END_ZONE_ID,
                            EXAM_DIRECTION AS DIRECTION,
                            EXAM_BEGIN_OFFSET AS BEGIN_OFFSET,
                            EXAM_END_OFFSET AS END_OFFSET,
			    EXAM_TRANSLATION_SPEED AS TRANSLATION_SPEED,
			    EXAM_ROTATION_SPEED AS ROTATION_SPEED,
                            EXAM_CREATION_DATE AS CREATION_DATE,
                            EXAM_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            EXAMS E'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetFiles`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the FILES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            F.FILE_ID AS ID,
							F.FILE_NAME AS NAME,
                            F.FILE_TYPE AS TYPE,
                            F.FILE_PATH AS PATH,
                            F.FILE_VERSION AS VERSION,
                            F.FILE_CREATION_DATE AS CREATION_DATE,
                            F.FILE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            FILES F '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the GEOMETRIC_ZONES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            GZ.GEOMETRIC_ZONE_ID AS ID,
                            GZ.GEOMETRIC_ZONE_STEAM_GENERATOR_TYPE_ID AS STEAM_GENERATOR_TYPE_ID,
                            GZ.GEOMETRIC_ZONE_TYPE_ID AS TYPE_ID,
                            GZ.GEOMETRIC_ZONE_NAME AS NAME,
                            GZ.GEOMETRIC_ZONE_LENGTH AS LENGTH,
                            GZ.GEOMETRIC_ZONE_SRZ_VALUE AS SRZ_VALUE,
                            GZ.GEOMETRIC_ZONE_LIMIT_PLUS AS LIMIT_PLUS,
                            GZ.GEOMETRIC_ZONE_LIMIT_MINUS AS LIMIT_MINUS,
                            GZ.GEOMETRIC_ZONE_OVERLAP AS OVERLAP, 
                            GZ.GEOMETRIC_ZONE_SIGN_CHANGE AS SIGN_CHANGE,
                            GZ.GEOMETRIC_ZONE_REPORT_REF AS REPORT_REF,
                            GZ.GEOMETRIC_ZONE_LEG_TYPE AS LEG_TYPE,
                            GZ.GEOMETRIC_ZONE_CREATION_DATE AS CREATION_DATE,
                            GZ.GEOMETRIC_ZONE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            GEOMETRIC_ZONES GZ'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetInspections`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the INSPECTIONS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            I.INSPECTION_ID AS ID,
			    I.INSPECTION_CONTRACTOR_ID AS CONTRACTOR_ID,
                            I.INSPECTION_SITE_ID AS SITE_ID,
                            I.INSPECTION_NUCLEAR_PLANT_UNIT_ID AS NUCLEAR_PLANT_UNIT_ID,
                            I.INSPECTION_UNIT_CONFIGURATION_ID AS UNIT_CONFIGURATION_ID,
                            I.INSPECTION_NAME AS NAME,
                            I.INSPECTION_NUMBER AS NUMBER,
                            I.INSPECTION_START_DATE AS START_DATE,
                            I.INSPECTION_FINISH_DATE AS FINISH_DATE,
                            I.INSPECTION_COMMENT AS COMMENT,
                            I.INSPECTION_CREATION_DATE AS CREATION_DATE,
                            I.INSPECTION_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            INSPECTIONS I'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetLayouts`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the LAYOUTS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            L.LAYOUT_ID AS ID,
                            L.LAYOUT_NAME AS NAME,
                            L.LAYOUT_FILE AS FILE,
                            L.LAYOUT_CREATION_DATE AS CREATION_DATE,
                            L.LAYOUT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            LAYOUTS L'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetManways`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the MANWAYS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            M.MANWAY_ID AS ID,
                            M.MANWAY_STEAM_GENERATOR_ID AS STEAM_GENERATOR_ID,
                            M.MANWAY_LEG_TYPE AS LEG_TYPE,
                            M.MANWAY_DIAMETER AS DIAMETER,
                            M.MANWAY_ROW AS ROW_POSITION,
                            M.MANWAY_COLUMN AS COLUMN_POSITION,
			    M.MANWAY_X AS X,
                            M.MANWAY_Y AS Y,
                            M.MANWAY_CREATION_DATE AS CREATION_DATE,
                            M.MANWAY_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            MANWAYS M'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetNuclearPlantUnits`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the NUCLEAR_PLANT_UNITS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            NPU.NUCLEAR_PLANT_UNIT_ID AS ID,
                            NPU.NUCLEAR_PLANT_UNIT_SITE_ID AS SITE_ID,
                            NPU.NUCLEAR_PLANT_UNIT_NUMBER AS NUMBER,
                            NPU.NUCLEAR_PLANT_UNIT_NAME AS NAME,
                            NPU.NUCLEAR_PLANT_UNIT_CREATION_DATE AS CREATION_DATE,
                            NPU.NUCLEAR_PLANT_UNIT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            NUCLEAR_PLANT_UNITS NPU'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetOrgans`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ORGANS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            O.ORGAN_ID AS ID,
                            O.ORGAN_TYPE AS TYPE,
                            O.ORGAN_SITE_ID AS SITE_ID,
                            O.ORGAN_MANUFACTURER_ID AS MANUFACTURER_ID,
                            O.ORGAN_NUCLEAR_PLANT_UNIT_ID AS NUCLEAR_PLANT_UNIT_ID ,
                            O.ORGAN_NAME AS NAME,
                            O.ORGAN_SERIAL_NUMBER AS SERIAL_NUMBER,
                            O.ORGAN_CREATION_DATE AS CREATION_DATE,
                            O.ORGAN_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ORGANS O'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTasks`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TASKS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            T.TASK_ID AS ID,
                            T.TASK_TYPE AS TYPE,
                            T.TASK_NAME AS NAME,
                            T.TASK_CONFIGURATION_FILE AS CONFIGURATION_FILE,
                            T.TASK_CREATION_DATE AS CREATION_DATE,
                            T.TASK_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            TASKS T'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTubesGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TUBES_GEOMETRIC_ZONES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            TGZ.TUBES_GEOMETRIC_ZONE_ID AS ID,
                            TGZ.TUBES_GEOMETRIC_ZONE_TUBE_ID AS TUBE_ID,
                            TGZ.TUBES_GEOMETRIC_ZONE_GEOMETRIC_ZONE_ID AS GEOMETRIC_ZONE_ID,
                            TGZ.TUBES_GEOMETRIC_ZONE_ALTITUDE AS ALTITUDE,
                            TGZ.TUBES_GEOMETRIC_ZONE_LENGTH AS LENGTH,
                            TGZ.TUBES_GEOMETRIC_ZONE_CREATION_DATE AS CREATION_DATE,
                            TGZ.TUBES_GEOMETRIC_ZONE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            TUBES_GEOMETRIC_ZONES TGZ'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetUsersRoles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the USERS_ROLES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            UR.USER_ID,
                            UR.ROLE_ID
                        FROM
                            USERS_ROLES UR'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpRaiseEnumValueNotFoundError`
(
IN enumName VARCHAR(100),
IN enumValue INT)
SQL SECURITY DEFINER
BEGIN
	
    CALL SpRaiseError( -1001, CONCAT( 'Enum value [', enumValue , '] not found on enum [', enumName, ']' ) );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddAcquisitionGroupLine`
(
IN acquisitionGroupId BIGINT,
IN acquisitionLineId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACQUISITION_GROUPS_LINES table'
BEGIN

    INSERT INTO ACQUISITION_GROUPS_LINES
    (
        ACQUISITION_GROUP_ID,
        ACQUISITION_LINE_ID
    )
    VALUES
    (
        acquisitionGroupId,
        acquisitionLineId
    );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAcquisition`
(
INOUT id BIGINT,
IN acquisitionGroupId BIGINT,
IN examId BIGINT,
IN cycleState Smallint,
IN acquisitionType Smallint,
IN pokOk TINYINT(1),
IN isAutomaticMode TINYINT(1),
IN qcState Smallint,
IN beginDate DATETIME,
IN finishDate DATETIME,
IN cycleName VARCHAR(200),
IN parentId BIGINT,
IN resumeReason VARCHAR(255),
IN resumeCode VARCHAR(20),
IN resumeTransmitter VARCHAR(100),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACQUISITION table or update an existing record'
BEGIN

    IF examId <= 0 THEN
	   SET examId = NULL;
	END IF;

	IF parentId <= 0 THEN
	   SET parentId = NULL;
	END IF;

    IF id IS NULL THEN

        INSERT INTO ACQUISITIONS
        (
            ACQUISITION_ACQUISITION_GROUP_ID,
            ACQUISITION_PARENT_ID,
            ACQUISITION_EXAM_ID,
            ACQUISITION_CYCLE_STATE,
            ACQUISITION_TYPE,
            ACQUISITION_POSITION_OK,
            ACQUISITION_IS_AUTOMATIC_MODE,
            ACQUISITION_QUALITY_CHECK_STATE,
            ACQUISITION_CYCLE_NAME,
            ACQUISITION_BEGIN_DATE,
            ACQUISITION_FINISH_DATE,
            ACQUISITION_RESUME_REASON,
            ACQUISITION_RESUME_CODE,
            ACQUISITION_RESUME_TRANSMITTER,
            ACQUISITION_CREATION_DATE
        )
        VALUES
        (
            acquisitionGroupId,
            parentId,
            examId,
            cycleState,
            acquisitionType,
            pokOk,
            isAutomaticMode,
            qcState,
            cycleName,
            beginDate,
            finishDate,
            resumeReason,
            resumeCode,
            resumeTransmitter,
            NOW()
        );
    
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ACQUISITIONS', 'ACQUISITION',  id, date );
    
    ELSE
    
        IF NOT FcAcquisitionExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ACQUISITIONS', id );
        END IF;

        UPDATE 
            ACQUISITIONS 
        SET
            ACQUISITION_ACQUISITION_GROUP_ID = acquisitionGroupId,
            ACQUISITION_PARENT_ID = parentId,
            ACQUISITION_CYCLE_STATE = cycleState,
            ACQUISITION_EXAM_ID = examId,
            ACQUISITION_TYPE = acquisitionType,
            ACQUISITION_POSITION_OK = pokOk,
            ACQUISITION_IS_AUTOMATIC_MODE = isAutomaticMode,
            ACQUISITION_QUALITY_CHECK_STATE = qcState,
            ACQUISITION_CYCLE_NAME = cycleName,
            ACQUISITION_BEGIN_DATE = beginDate,
            ACQUISITION_FINISH_DATE = finishDate,
            ACQUISITION_RESUME_REASON = resumeReason,
            ACQUISITION_RESUME_CODE = resumeCode,
            ACQUISITION_RESUME_TRANSMITTER = resumeTransmitter,
            ACQUISITION_MODIFICATION_DATE = NOW()
        WHERE
            ACQUISITION_ID = id;
    
        CALL SpGetModificationDate('ACQUISITIONS', 'ACQUISITION',  id, date );
    END IF;
    
    IF parentId IS NOT NULL THEN
        
        UPDATE 
            ACQUISITIONS
        SET
            ACQUISITION_CHILD_ID = id
        WHERE
            ACQUISITION_ID = parentId;
            
    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAcquisitionFile`
(
INOUT id BIGINT,
IN acquisitionId BIGINT,
IN fileType SMALLINT,
IN name VARCHAR(255),
IN transferDate DATETIME,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACQUISITION_FILES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO ACQUISITION_FILES
        (
            ACQUISITION_FILE_ACQUISITION_ID,
            ACQUISITION_FILE_TYPE,
            ACQUISITION_FILE_NAME,
            ACQUISITION_FILE_TRANSFER_DATE,
            ACQUISITION_FILE_CREATION_DATE
        )
        VALUES
        (
            acquisitionId,
            fileType,
            name,
            transferDate,
            NOW()
        );
          
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ACQUISITION_FILES', 'ACQUISITION_FILE',  id, date );
    
    ELSE
    
        IF NOT FcAcquisitionFileExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ACQUISITION_FILES', id );
        END IF;

        UPDATE 
            ACQUISITION_FILES 
        SET
            ACQUISITION_FILE_ACQUISITION_ID = acquisitionId,
            ACQUISITION_FILE_TYPE = fileType,
            ACQUISITION_FILE_NAME = name,
            ACQUISITION_FILE_TRANSFER_DATE = transferDate,
            ACQUISITION_FILE_MODIFICATION_DATE =  NOW()
        WHERE
            ACQUISITION_FILE_ID = id;
            
        CALL SpGetModificationDate('ACQUISITION_FILES', 'ACQUISITION_FILE',  id, date );
    
    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAcquisitionGroup`
(
INOUT id BIGINT,
IN controlId BIGINT,
IN groupState Smallint,
IN number INT,
IN name VARCHAR(100),
IN directory VARCHAR(255),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACQUISITION_GROUPS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO ACQUISITION_GROUPS
        (
            ACQUISITION_GROUP_CONTROL_ID,
            ACQUISITION_GROUP_STATE,
            ACQUISITION_GROUP_NUMBER,
            ACQUISITION_GROUP_NAME,
            ACQUISITION_GROUP_DIRECTORY,
            ACQUISITION_GROUP_CREATION_DATE
        )
        VALUES
        (
            controlId,
            groupState,
            number,
            name,
            directory,
            NOW()
        );
    
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ACQUISITION_GROUPS', 'ACQUISITION_GROUP',  id, date );
    
    ELSE
    
        IF NOT FcAcquisitionGroupExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ACQUISITION_GROUPS', id );
        END IF;

        UPDATE 
            ACQUISITION_GROUPS 
        SET
            ACQUISITION_GROUP_CONTROL_ID = controlId,
            ACQUISITION_GROUP_STATE = groupState,
            ACQUISITION_GROUP_NUMBER = number,
            ACQUISITION_GROUP_NAME = name,
            ACQUISITION_GROUP_DIRECTORY = directory,
            ACQUISITION_GROUP_MODIFICATION_DATE = NOW()
        WHERE
            ACQUISITION_GROUP_ID = id;
            
        CALL SpGetModificationDate('ACQUISITION_GROUPS', 'ACQUISITION_GROUP',  id, date );
    
    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAcquisitionLine`
(
INOUT id BIGINT,
IN number INT,
IN label VARCHAR(255),
IN color INT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACQUISITION_LINES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO ACQUISITION_LINES 
        (
            ACQUISITION_LINE_NUMBER,
            ACQUISITION_LINE_LABEL,
            ACQUISITION_LINE_COLOR,
            ACQUISITION_LINE_CREATION_DATE
        )
        VALUES
        (
            number,
            label,
            color,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ACQUISITION_LINES', 'ACQUISITION_LINE',  id, date );

    ELSE
    
        IF NOT FcAcquisitionLineExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ACQUISITION_LINES', id );
        END IF;

        UPDATE 
            ACQUISITION_LINES 
        SET
            ACQUISITION_LINE_NUMBER = number,
            ACQUISITION_LINE_LABEL = label,
            ACQUISITION_LINE_COLOR = color,
            ACQUISITION_LINE_MODIFICATION_DATE = NOW()
        WHERE
            ACQUISITION_LINE_ID = id;
            
        CALL SpGetModificationDate('ACQUISITION_LINES', 'ACQUISITION_LINE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionGroupsLinesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACQUISITION_GROUPS_LINES table'
BEGIN

    CALL SpDeleteAcquisitionGroupsLines( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionGroupLine`
(
IN acquisitionGroupId BIGINT,
IN acquisitionLineId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACQUISITION_GROUPS_LINES table'
BEGIN

    DELETE FROM 
        ACQUISITION_GROUPS_LINES
    WHERE
        ACQUISITION_GROUP_ID = acquisitionGroupId
    AND
        ACQUISITION_LINE_ID = acquisitionLineId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionGroupsLines`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACQUISITION_GROUPS_LINES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACQUISITION_GROUPS_LINES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionLine`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACQUISITION_LINES table'
BEGIN

    DELETE FROM 
        ACQUISITION_LINES
    WHERE
        ACQUISITION_LINE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionGroupsLines`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITION_GROUPS_LINES table matching the where clause.'
BEGIN

        CALL SpGetRecords( 'SELECT
                                AGL.ACQUISITION_GROUP_ID,
                                AGL.ACQUISITION_LINE_ID
                            FROM
                                ACQUISITION_GROUPS_LINES AGL'
                        ,whereClause ); 
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearAcquisitionLinesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACQUISITION_LINES table'
BEGIN

    CALL SpDeleteAcquisitionLines( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteAcquisitionLines`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACQUISITION_LINES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACQUISITION_LINES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionLines`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITION_LINES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            AL.ACQUISITION_LINE_ID AS ID,
                            AL.ACQUISITION_LINE_NUMBER AS NUMBER,
                            AL.ACQUISITION_LINE_LABEL AS LABEL,
                            AL.ACQUISITION_LINE_COLOR AS COLOR,
                            AL.ACQUISITION_LINE_CREATION_DATE AS CREATION_DATE,
                            AL.ACQUISITION_LINE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ACQUISITION_LINES AL'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetApplicationFunctionalitiesRoles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the APPLICATION_FUNCTIONALITIES_ROLES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            AFR_APPLICATION_FUNCTIONALITY_ID AS APPLICATION_FUNCTIONALITY_ID,
                            AFR_ROLE_ID AS ROLE_ID,
                            AFR_CREATION_DATE AS CREATION_DATE
                        FROM
                            APPLICATION_FUNCTIONALITIES_ROLES AFR'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetBackupHistories`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the BACKUP_HISTORIES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            BH.BACKUP_HISTORY_ID AS ID,
                            BH.BACKUP_HISTORY_DATABASE_VERSION AS DATABASE_VERSION,
                            BH.BACKUP_HISTORY_FILE AS FILE,
                            BH.BACKUP_HISTORY_COMMENT AS COMMENT,
                            BH.BACKUP_HISTORY_CREATION_DATE AS CREATION_DATE,
                            BH.BACKUP_HISTORY_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            BACKUP_HISTORIES BH'
                    ,whereClause );  

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetFMECheckListItems`
()
SQL SECURITY DEFINER
COMMENT 'Retrieves all records which represent FME Check list items present in the ATTRIBUTES tabme'
BEGIN

        SELECT
            A.ATTRIBUTE_ID AS ID,
            A.ATTRIBUTE_LABEL AS LABEL,
            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
        FROM
            ATTRIBUTES A
        WHERE
            ATTRIBUTE_PARENT_ID = FcGetAttributeId( 'FME_CHECK_LIST_ITEMS' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetRoles`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ROLES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            R.ROLE_ID AS ID,
                            R.ROLE_NAME AS NAME,
                            R.ROLE_LEVEL AS LEVEL,
                            R.ROLE_CREATION_DATE AS CREATION_DATE,
                            R.ROLE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ROLES R'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetCreationDate`
(
tableName VARCHAR(200),
columnPrefixe VARCHAR(200),
id BIGINT,
OUT creationDate DATETIME)
SQL SECURITY DEFINER
BEGIN

    SET @strQuery = CONCAT( 'SELECT ', columnPrefixe, '_CREATION_DATE INTO @tmp FROM ', tableName , ' WHERE ', columnPrefixe, '_ID = ', id);

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    
    SET creationDate = @tmp;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetDictionaries`
()
SQL SECURITY DEFINER
BEGIN

select  l.LANGUAGE_NAME AS LANGUAGE_NAME,
        IFNULL(c.COMPONENT_NAME,''),
        IFNULL(c.COMPONENT_CODE,'')
from 
texts t 
  inner join languages l on t.TEXT_LANGUAGE_ID = l.LANGUAGE_ID 
  left join components c on t.TEXT_COMPONENT_ID = c.COMPONENT_ID
  left join attributes tg on t.TEXT_GROUP_ID = tg.ATTRIBUTE_ID
group by
        LANGUAGE_NAME,
        COMPONENT_NAME,
        COMPONENT_CODE;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetModificationDate`
(
tableName VARCHAR(200),
columnPrefixe VARCHAR(200),
id BIGINT,
OUT modificationDate DATETIME)
SQL SECURITY DEFINER
BEGIN

    SET @strQuery = CONCAT( 'SELECT ', columnPrefixe, '_MODIFICATION_DATE INTO @tmp FROM ', tableName , ' WHERE ', columnPrefixe, '_ID = ', id);

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    
    SET modificationDate = @tmp;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpSetTranslation`
(
IN languageName varchar(50), 
IN componentName varchar(255), 
IN componentCode varchar(10), 
IN textKey varchar(100), 
IN textGroup varchar(100), 
IN textValue longtext, 
IN errorCode varchar(10), 
IN groupCode varchar(10) 
)
SQL SECURITY DEFINER
COMMENT 'Allows to add or update a record in TEXTS table and corresponding item in LANGUAGES, ATTRIBUTES and COMPONENTS if needed'
BEGIN

    DECLARE langID INTEGER DEFAULT NULL;
    DECLARE assemblyID INTEGER DEFAULT NULL;
    DECLARE groupID INTEGER DEFAULT NULL;
    DECLARE textID INTEGER DEFAULT NULL;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK; 

    SELECT 
        LANGUAGE_ID 
    INTO 
        langID 
    FROM 
        LANGUAGES l 
    WHERE 
        l.LANGUAGE_NAME = languageName;

    IF langID is NULL THEN
        CALL SpAddUpdateLanguage(langID, languageName, '');
    END IF;

    SELECT 
        COMPONENT_ID
    FROM 
        COMPONENTS 
    WHERE 
        COMPONENT_NAME = componentName
    INTO 
        assemblyID;

    IF assemblyID IS NULL AND componentName IS NOT NULL AND componentName <> '' THEN 
        CALL SpAddUpdateComponent(assemblyID, NULL, 0, componentName, '', '',componentCode);
    END IF;

	set groupID = FcGetAttributeId(concat('LOCALIZATION_GROUP.',textGroup)) ;

    IF groupID IS NULL AND textGroup IS NOT NULL AND textGroup <> '' THEN 
        CALL SpAddUpdateAttribute(groupID, FcGetAttributeId('LOCALIZATION_GROUP'),NULL,NULL,NULL,textGroup,'', '','', 0, '','','','');
 
    END IF;

    CALL SpAddUpdateText(textID, assemblyID, groupID, langID,  textKey, textValue, errorCode, groupCode);

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentCategory`
(
INOUT id BIGINT,
IN parentId BIGINT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN isOptional TINYINT(1),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (EQUIPMENT_CATEGORIES) virtual table'
BEGIN

    DECLARE parentIdTmp INT DEFAULT NULL;
    
    IF parentId <= 0 THEN
        SET parentId = NULL;
    END IF;

    IF parentId IS NULL THEN
    
        SET parentIdTmp = FcGetAttributeId( 'EQUIPMENT_CATEGORIES' );
    
    ELSE
    
        SET parentIdTmp = parentId;
    
    END IF;

    CALL SpAddUpdateAttribute( id,
                               parentIdTmp,
                               NULL,
                               NULL,
                               NULL,
                               name,
                               label,
                               labelTranslationKey,
                               NULL,
                               0,
                               NULL,
                               isOptional,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTubeMaterial`
(
INOUT id BIGINT,
IN name VARCHAR(30),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (TUBE_MATERIAL_NAME) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'TUBE_MATERIAL_NAME' ),
                               NULL,
                               NULL,
                               NULL,
                               name,
                               name,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTubeMaterial`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTubeMaterials`
()
SQL SECURITY DEFINER
COMMENT 'Allows to delete existing records in the ATTRIBUTES table, Delete all record where PARENT_FULL_NAME= ''TUBE_MATERIAL_NAME'''
BEGIN

    CALL SpDeleteAttributes( 'WHERE ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'TUBE_MATERIAL_NAME\' )' );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSgTubesGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SG_TUBES_GEOMETRIC_ZONES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            SGTGZ.SG_TUBES_GEOMETRIC_ZONE_ID AS TUBES_GEOMETRIC_ZONE_ID,
                            SGTGZ.SG_TUBES_GEOMETRIC_ZONE_STEAM_GENERATOR_ID AS STEAM_GENERATOR_ID,
                            O.ORGAN_NAME AS STEAM_GENERATOR_NAME,
                            TGZ.TUBES_GEOMETRIC_ZONE_TUBE_ID AS TUBE_ID,
                            T.TUBE_ROW_HOT_LEG AS ROW_HOT_LEG,
                            T.TUBE_COLUMN_HOT_LEG AS COLUMN_HOT_LEG,
                            T.TUBE_ROW_COLD_LEG AS ROW_COLD_LEG,
                            T.TUBE_COLUMN_COLD_LEG AS COLUMN_COLD_LEG,
                            SGTGZ.SG_TUBES_GEOMETRIC_ZONE_ALTITUDE AS ALTITUDE,
                            SGTGZ.SG_TUBES_GEOMETRIC_ZONE_CREATION_DATE AS CREATION_DATE,
                            SGTGZ.SG_TUBES_GEOMETRIC_ZONE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            SG_TUBES_GEOMETRIC_ZONES SGTGZ
                                INNER JOIN TUBES_GEOMETRIC_ZONES TGZ ON TGZ.TUBES_GEOMETRIC_ZONE_ID = SGTGZ.SG_TUBES_GEOMETRIC_ZONE_ID
                                INNER JOIN TUBES T ON T.TUBE_ID = TGZ.TUBES_GEOMETRIC_ZONE_TUBE_ID
                                INNER JOIN ORGANS O ON O.ORGAN_ID = SGTGZ.SG_TUBES_GEOMETRIC_ZONE_STEAM_GENERATOR_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTubeMaterials`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            IF(A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'TUBE_MATERIAL_NAME\' ),NULL, A.ATTRIBUTE_PARENT_ID ) AS PARENT_ID,
                            A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
                            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
                            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
                            A.ATTRIBUTE_VALUE1 AS VALUE1,
                            A.ATTRIBUTE_VALUE2 AS VALUE2,
                            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
						 WHERE A.ATTRIBUTE_FULL_NAME LIKE \'TUBE_MATERIAL_NAME.%\''
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetIos`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the IOS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            IOS.IO_ID AS ID,
                            IOS.IO_DATA_TYPE AS DATA_TYPE,
                            IOS.IO_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
                            IOS.IO_UNIT_ID AS UNIT_ID,
                            IOS.IO_NAME AS NAME,
                            IOS.IO_IS_INPUT AS IS_INPUT,
                            IOS.IO_TECHNICAL_ADDRESS AS TECHNICAL_ADDRESS,
                            IOS.IO_CREATION_DATE AS CREATION_DATE,
                            IOS.IO_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            IOS '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipments`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENTS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            E.EQUIPMENT_ID AS ID,
                            E.EQUIPMENT_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
                            E.EQUIPMENT_CONDITION AS ECONDITION,
                            E.EQUIPMENT_FME_STATUS AS FME_STATUS,
                            E.EQUIPMENT_STATE AS STATE,
                            E.EQUIPMENT_SERIAL_NUMBER AS SERIAL_NUMBER,
                            E.EQUIPMENT_NAME AS NAME,
                            E.EQUIPMENT_CONNECTION_STRING AS CONNECTION_STRING,
                            E.EQUIPMENT_MODULE_CONNECTION_STRING AS MODULE_CONNECTION_STRING,
                            E.EQUIPMENT_VERSION AS VERSION,
                            E.EQUIPMENT_IS_TEMPLATE AS IS_TEMPLATE,
                            E.EQUIPMENT_CUSTOM_ATTRIBUTES_LAYOUT_FILE AS CUSTOM_ATTRIBUTES_LAYOUT_FILE,
                            E.EQUIPMENT_COMMENT AS COMMENT,
                            E.EQUIPMENT_CREATION_DATE AS CREATION_DATE,
                            E.EQUIPMENT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            EQUIPMENTS E'
                    ,whereClause );                      

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateOrganAttribute`
(
IN organId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ORGANS_ATTRIBUTES table or update an existing record'
BEGIN

	SET date = NOW();
    IF FcOrganAttributeExist( organId, attributeId ) = FALSE THEN
 		
        INSERT INTO ORGANS_ATTRIBUTES 
        (
            ORGAN_ATTRIBUTE_ORGAN_ID,
            ORGAN_ATTRIBUTE_ATTRIBUTE_ID,
            ORGAN_ATTRIBUTE_VALUE,
            ORGAN_ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            organId,
            attributeId,
            value,
            date
        );
     
    ELSE
    
        UPDATE
            ORGANS_ATTRIBUTES 
        SET
            ORGAN_ATTRIBUTE_VALUE = value,
            ORGAN_ATTRIBUTE_MODIFICATION_DATE = date
        WHERE
            ORGAN_ATTRIBUTE_ORGAN_ID = organId
        AND
            ORGAN_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTubesGeometricZone`
(
INOUT id BIGINT,
IN tubeId BIGINT,
IN geometricZoneId BIGINT,
IN altitude FLOAT,
IN length FLOAT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TUBES_GEOMETRIC_ZONES table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO TUBES_GEOMETRIC_ZONES 
        (
            TUBES_GEOMETRIC_ZONE_TUBE_ID,
            TUBES_GEOMETRIC_ZONE_GEOMETRIC_ZONE_ID,
            TUBES_GEOMETRIC_ZONE_ALTITUDE,
            TUBES_GEOMETRIC_ZONE_LENGTH,
            TUBES_GEOMETRIC_ZONE_CREATION_DATE
        )
        VALUES
        (
            tubeId,
            geometricZoneId,
            altitude,
            length,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('TUBES_GEOMETRIC_ZONES', 'TUBES_GEOMETRIC_ZONE',  id, date );

    ELSE
    
        IF NOT FcTubeGeometricZoneExist( id ) THEN
            CALL SpRaiseRecordNotFound( 'TUBES_GEOMETRIC_ZONES', id );
        END IF;

        UPDATE 
            TUBES_GEOMETRIC_ZONES 
        SET
            TUBES_GEOMETRIC_ZONE_TUBE_ID = tubeId,
            TUBES_GEOMETRIC_ZONE_GEOMETRIC_ZONE_ID = geometricZoneId,
            TUBES_GEOMETRIC_ZONE_ALTITUDE = altitude,
            TUBES_GEOMETRIC_ZONE_LENGTH = length,
            TUBES_GEOMETRIC_ZONE_MODIFICATION_DATE = NOW()
        WHERE
            TUBES_GEOMETRIC_ZONE_ID = id;
            
        CALL SpGetModificationDate('TUBES_GEOMETRIC_ZONES', 'TUBES_GEOMETRIC_ZONE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTubesZone`
(
IN TubeId BIGINT,
IN ZoneId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TUBES_ZONES table or update an existing record'
BEGIN

    IF NOT FcTubeZoneExist( TubeId, ZoneId ) THEN

        INSERT INTO TUBES_ZONES 
        (
            TUBE_ZONE_TUBE_ID, 
            TUBE_ZONE_ZONE_ID,           
            TUBE_ZONE_CREATION_DATE
        )
        VALUES
        (
            TubeId,
            ZoneId,
            NOW()
        );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentCategory`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteIo`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the IOS table'
BEGIN

    DELETE FROM 
        IOS
    WHERE
        IO_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteOrganAttribute`
(
IN organId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ORGANS_ATTRIBUTES table matching the whereClause'
BEGIN

    DELETE FROM
		ORGANS_ATTRIBUTES
	WHERE
		ORGAN_ATTRIBUTE_ORGAN_ID = organId
	AND
		ORGAN_ATTRIBUTE_ATTRIBUTE_ID = attributeId;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTubesGeometricZone`
(
IN id BIGINT)
SQL SECURITY DEFINER
BEGIN

    DELETE FROM
        TUBES_GEOMETRIC_ZONES
    WHERE
        TUBES_GEOMETRIC_ZONE_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTubeZone`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TUBES_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'TUBES_ZONES', whereClause );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesOrgan`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'ORGANS_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'ORGANS_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesSteamGenerator`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	CALL SpGetAttributesOrgan ( CONCAT( 'OR A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'STEAM_GENERATORS_ATTRIBUTES\' )', ' ', whereClause  ) );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentCategories`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            IF(A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'EQUIPMENT_CATEGORIES\' ),NULL, A.ATTRIBUTE_PARENT_ID ) AS PARENT_ID,
                            A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
                            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
                            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
                            A.ATTRIBUTE_VALUE1 AS IS_OPTIONAL,
                            A.ATTRIBUTE_VALUE2 AS VALUE2,
                            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
						 WHERE A.ATTRIBUTE_FULL_NAME LIKE \'EQUIPMENT_CATEGORIES.%\''
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetOrganAttributes`
(
IN organId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ORGANS_ATTRIBUTES table matching the where clause.'
BEGIN     
		
		SELECT
            OA.ORGAN_ATTRIBUTE_ORGAN_ID AS ORGAN_ID,
            OA.ORGAN_ATTRIBUTE_ATTRIBUTE_ID AS ID,
            OA.ORGAN_ATTRIBUTE_VALUE AS VALUE,
            OA.ORGAN_ATTRIBUTE_CREATION_DATE AS VALUE_CREATION_DATE,
            OA.ORGAN_ATTRIBUTE_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE,
            A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
            A.ATTRIBUTE_NAME AS NAME,
            A.ATTRIBUTE_LABEL AS LABEL,
            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
            A.ATTRIBUTE_NOT_NULL AS NOT_NULL,
            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
            A.ATTRIBUTE_VALUE1 AS VALUE1,
            A.ATTRIBUTE_VALUE2 AS VALUE2,
            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
        FROM
                ORGANS_ATTRIBUTES OA
                    INNER JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = OA.ORGAN_ATTRIBUTE_ATTRIBUTE_ID
        WHERE  OA.ORGAN_ATTRIBUTE_ORGAN_ID = organId;                     

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTubesByZones`
(
IN zoneId INT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TUBES_ZONES table matching the zoneId.'
BEGIN

SELECT
        TZ.TUBE_ZONE_TUBE_ID AS TUBE_ID
    FROM
        TUBES_ZONES TZ
    WHERE
        TZ.TUBE_ZONE_ZONE_ID = zoneId;                     

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetZonesByTubes`
(
IN tubeId INT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TUBES_ZONES table matching the tubeId.'
BEGIN

SELECT
        TZ.TUBE_ZONE_ZONE_ID AS ZONE_ID
    FROM
        TUBES_ZONES TZ
    WHERE
        TZ.TUBE_ZONE_TUBE_ID = tubeId;                     

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddInspectionSelectedControl`
(
IN inspectionId BIGINT,
IN controlId BIGINT,
IN acquisitionLineId BIGINT,
IN host VARCHAR(20)
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SELECTED_CONTROLS table or update an existing record'
BEGIN

    INSERT INTO SELECTED_CONTROLS 
    (
        SELECTED_CONTROL_INSPECTION_ID,
        SELECTED_CONTROL_CONTROL_ID,
        SELECTED_CONTROL_ACQUISITION_LINE_ID,
        SELECTED_CONTROL_HOST,
		SELECTED_CONTROL_CREATION_DATE
    )
    VALUES
    (
        inspectionId,
        controlId,
        acquisitionLineId,
		host,
        NOW()
    );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAttributeEquipmentSpecific`
(
INOUT id BIGINT,
IN unitId BIGINT,
IN dataType INT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN description TEXT,
IN defaultValue VARCHAR(255),
IN value1 VARCHAR(255),
IN value2 VARCHAR(255),
IN valueFormat VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (EQUIPMENT_SPECIFIC_ATTRIBUTES) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'EQUIPMENT_SPECIFIC_ATTRIBUTES' ),
                               NULL,
                               unitId,
                               dataType,
                               name,
                               label,
                               labelTranslationKey,
                               description,
                               0,
                               defaultValue,
                               value1,
                               value2,
                               valueFormat,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAttributeEquipmentState`
(
INOUT id BIGINT,
IN unitId BIGINT,
IN dataType INT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN description TEXT,
IN defaultValue VARCHAR(255),
IN value1 VARCHAR(255),
IN value2 VARCHAR(255),
IN valueFormat VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (EQUIPMENT_STATE_ATTRIBUTES) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'EQUIPMENT_STATE_ATTRIBUTES' ),
                               NULL,
                               unitId,
                               dataType,
                               name,
                               label,
                               labelTranslationKey,
                               description,
                               0,
                               defaultValue,
                               value1,
                               value2,
                               valueFormat,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAttributeEquipmentStatistic`
(
INOUT id BIGINT,
IN unitId BIGINT,
IN dataType INT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN description TEXT,
IN defaultValue VARCHAR(255),
IN value1 VARCHAR(255),
IN value2 VARCHAR(255),
IN valueFormat VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (EQUIPMENT_STATISTIC_ATTRIBUTES) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'EQUIPMENT_STATISTIC_ATTRIBUTES' ),
                               NULL,
                               unitId,
                               dataType,
                               name,
                               label,
                               labelTranslationKey,
                               description,
                               0,
                               defaultValue,
                               value1,
                               value2,
                               valueFormat,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAttributeEquipmentTracking`
(
INOUT id BIGINT,
IN unitId BIGINT,
IN dataType INT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN description TEXT,
IN defaultValue VARCHAR(255),
IN value1 VARCHAR(255),
IN value2 VARCHAR(255),
IN valueFormat VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (EQUIPMENT_TRACKING_ATTRIBUTES) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'EQUIPMENT_TRACKING_ATTRIBUTES' ),
                               NULL,
                               unitId,
                               dataType,
                               name,
                               label,
                               labelTranslationKey,
                               description,
                               0,
                               defaultValue,
                               value1,
                               value2,
                               valueFormat,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateAttributeEquipmentType`
(
INOUT id BIGINT,
IN unitId BIGINT,
IN dataType INT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
IN description TEXT,
IN defaultValue VARCHAR(255),
IN value1 VARCHAR(255),
IN value2 VARCHAR(255),
IN valueFormat VARCHAR(255),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (EQUIPMENT_TYPE_ATTRIBUTES) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'EQUIPMENT_TYPE_ATTRIBUTES' ),
                               NULL,
                               unitId,
                               dataType,
                               name,
                               label,
                               labelTranslationKey,
                               description,
                               0,
                               defaultValue,
                               value1,
                               value2,
                               valueFormat,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateControlSelectedEquipment`
(
IN controlId BIGINT,
IN equipmentId BIGINT,
IN acquisitionLineId INT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SELECTED_EQUIPMENTS table or update an existing record'
BEGIN

    IF NOT FcControlSelectedEquipmentExist( controlId, equipmentId ) THEN

        INSERT INTO SELECTED_EQUIPMENTS 
        (
            SELECTED_EQUIPMENT_CONTROL_ID,
            SELECTED_EQUIPMENT_EQUIPMENT_ID,
            SELECTED_EQUIPMENT_ACQUISITION_LINE_ID,
            SELECTED_EQUIPMENT_CREATION_DATE
        )
        VALUES
        (
            controlId,
            equipmentId,
            acquisitionLineId,
			NOW()
        );

    ELSE

        UPDATE 
            SELECTED_EQUIPMENTS 
        SET
            SELECTED_EQUIPMENT_ACQUISITION_LINE_ID = acquisitionLineId,
            SELECTED_CONTROL_MODIFICATION_DATE = NOW()
        WHERE
            SELECTED_EQUIPMENT_EQUIPMENT_ID = equipmentId
        AND
            SELECTED_EQUIPMENT_CONTROL_ID = controlId;

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateControlSteamGenerator`
(
INOUT id BIGINT,
IN inspectionId BIGINT,
IN organId BIGINT,
IN linkedId BIGINT,
IN techniqueId BIGINT,
IN state SMALLINT,
IN mode SMALLINT,
IN controlOption SMALLINT,
IN name VARCHAR(100),
IN orderNumber INT,
IN procedure1 VARCHAR(100),
IN procedure2 VARCHAR(100),
IN startDate DATETIME,
IN finishDate DATETIME,
IN comment TEXT,
IN dataFormat SMALLINT,
IN sectorId BIGINT,
IN beginZoneId BIGINT,
IN endZoneId BIGINT,
IN leg SMALLINT,
IN saveAllData TINYINT(1),
IN beginOffset FLOAT,
IN endOffset FLOAT,
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the CONTROLS table or update an existing record'
BEGIN

	IF sectorId <= 0 THEN
		SET sectorId = NULL;
	END IF;

	IF endZoneId <= 0 THEN
		SET endZoneId = NULL;
	END IF;

	IF linkedId <= 0 THEN
		SET linkedId = NULL;
	END IF;
	
    IF id IS NULL THEN

        INSERT INTO CONTROLS 
        (
            CONTROL_INSPECTION_ID,
            CONTROL_ORGAN_ID,
			CONTROL_LINKED_ID,
            CONTROL_BEGIN_ZONE_ID,
            CONTROL_END_ZONE_ID,
            CONTROL_TECHNIQUE_ID,
            CONTROL_SECTOR_ID,
            CONTROL_LEG_TYPE,
            CONTROL_STATE,
            CONTROL_MODE,
            CONTROL_OPTION,
            CONTROL_NAME,
            CONTROL_ORDER,
            CONTROL_PROCEDURE1,
            CONTROL_PROCEDURE2,
            CONTROL_START_DATE,
            CONTROL_FINISH_DATE,
            CONTROL_COMMENT,
            CONTROL_DATA_FORMAT,
            CONTROL_BEGIN_OFFSET,
            CONTROL_END_OFFSET,
            CONTROL_SAVE_ALL_DATA,
            CONTROL_CREATION_DATE
        )
        VALUES
        (
            inspectionId,
            organId,
			linkedId,
            beginZoneId,
            endZoneId,
            techniqueId,
            sectorId,
            leg,
            state,
            mode,
            controlOption,
            name,
            orderNumber,
            procedure1,
            procedure2,
            startDate,
            finishDate,
            comment,
            dataFormat,
            beginOffset,
            endOffset,
            saveAllData,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('CONTROLS', 'CONTROL',  id, date );

    ELSE
    
        IF NOT FcControlExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'CONTROLS', id );
        END IF;

        UPDATE 
            CONTROLS 
        SET
            CONTROL_INSPECTION_ID = inspectionId,
            CONTROL_ORGAN_ID = organId,
			CONTROL_LINKED_ID = linkedId,
            CONTROL_BEGIN_ZONE_ID = beginZoneId,
            CONTROL_END_ZONE_ID = endZoneId,
            CONTROL_TECHNIQUE_ID = techniqueId,
            CONTROL_SECTOR_ID = sectorId,
            CONTROL_LEG_TYPE = leg,
            CONTROL_STATE = state,
            CONTROL_MODE = mode,
            CONTROL_OPTION = controlOption,
            CONTROL_NAME = name,
            CONTROL_ORDER = orderNumber,
            CONTROL_PROCEDURE1 = procedure1,
            CONTROL_PROCEDURE2  = procedure2,
            CONTROL_START_DATE = startDate,
            CONTROL_FINISH_DATE = finishDate,
            CONTROL_COMMENT = comment,
            CONTROL_DATA_FORMAT = dataFormat,
            CONTROL_BEGIN_OFFSET = beginOffset,
            CONTROL_END_OFFSET = endOffset,
            CONTROL_SAVE_ALL_DATA = saveAllData,
            CONTROL_MODIFICATION_DATE = NOW()
        WHERE
            CONTROL_ID = id;
            
        CALL SpGetModificationDate('CONTROLS', 'CONTROL',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEquipmentTrackingAttribute`
(
IN equipmentTrackingId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EQUIPMENT_TRACKINGS_ATTRIBUTES table or update an existing record'
BEGIN

    IF FcEquipmentTrackingAttributeExist( equipmentTrackingId, attributeId ) = FALSE THEN
  
        INSERT INTO EQUIPMENT_TRACKINGS_ATTRIBUTES 
        (
			ETA_EQUIPMENT_TRACKING_ID,
            ETA_ATTRIBUTE_ID,
            ETA_VALUE,
            ETA_CREATION_DATE
        )
        VALUES
        (
            equipmentTrackingId,
            attributeId,
            value,
            NOW()
        );
        CALL SpGetCreationDateFromLinkedTable( 'EQUIPMENT_TRACKINGS_ATTRIBUTES', 'ETA',  CONCAT( ' WHERE ETA_EQUIPMENT_TRACKING_ID = ', equipmentTrackingId, ' AND ETA_ATTRIBUTE_ID = ', attributeId), date );
    ELSE
    
        UPDATE
            EQUIPMENT_TRACKINGS_ATTRIBUTES 
        SET
            ETA_VALUE = value,
            ETA_MODIFICATION_DATE = NOW()
        WHERE
            ETA_EQUIPMENT_TRACKING_ID = equipmentTrackingId
        AND
            ETA_ATTRIBUTE_ID = attributeId;

        CALL SpGetModificationDateFromLinkedTable( 'EQUIPMENT_TRACKINGS_ATTRIBUTES', 'ETA',  CONCAT( ' WHERE ETA_EQUIPMENT_TRACKING_ID = ', equipmentTrackingId, ' AND ETA_ATTRIBUTE_ID = ', attributeId), date );
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateEvent`
(
INOUT id BIGINT,
IN level INT,
IN type INT,
IN controlId BIGINT,
IN lineNumber INT,
IN acquisitionGroupId BIGINT,
IN code VARCHAR(20),
IN acquitDate DATETIME,
IN source VARCHAR(100),
IN description TEXT,
IN detail1 TEXT,
IN detail2 TEXT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EVENTS table'
BEGIN

    DECLARE dbAcquisitionGroupId BIGINT DEFAULT NULL;
	DECLARE dbControlId BIGINT DEFAULT NULL;
		
	IF acquisitionGroupId > 0 THEN
		SET dbAcquisitionGroupId = acquisitionGroupId;
	END IF;

	IF controlId > 0 THEN
		SET dbControlId = controlId;
	END IF;

    IF id IS NULL THEN

		INSERT INTO EVENTS
		(
			EVENT_LEVEL,
			EVENT_TYPE,
			EVENT_CONTROL_ID,
			EVENT_LINE_NUMBER,
			EVENT_ACQUISITION_GROUP_ID,
			EVENT_CODE,
			EVENT_ACQUIT_DATE,
			EVENT_SOURCE,
			EVENT_DESCRIPTION,
			EVENT_DETAIL1,
			EVENT_DETAIL2,
            EVENT_CREATION_DATE
		)
		VALUES
		(
			level,
			type,
			dbControlId,
			lineNumber,
			dbAcquisitionGroupId,
			code,
			acquitDate,
			source,
			description,
			detail1,
			detail2,
            NOW()
		);
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('EVENTS', 'EVENT',  id, date );

    ELSE
    
        IF NOT FcEventExist( id ) THEN
            CALL SpRaiseRecordNotFound( 'EVENTS', id );
        END IF;

        UPDATE 
            EVENTS 
        SET
            EVENT_LEVEL = level,
			EVENT_TYPE = type,
			EVENT_CONTROL_ID = dbControlId,
			EVENT_LINE_NUMBER = lineNumber,
			EVENT_ACQUISITION_GROUP_ID = dbAcquisitionGroupId,
			EVENT_CODE = code,
			EVENT_ACQUIT_DATE = acquitDate,
			EVENT_SOURCE = source,
			EVENT_DESCRIPTION = description,
			EVENT_DETAIL1 = detail1,
			EVENT_DETAIL2 = detail2,
            EVENT_MODIFICATION_DATE = NOW()
        WHERE
            EVENT_ID = id;
            
        CALL SpGetModificationDate('EVENTS', 'EVENT',  id, date );

    END IF;

    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlSelectedEquipment`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SELECTED_EQUIPMENTS table matching the whereClause'
BEGIN
	
	CALL SpDeleteRecords( 'SELECTED_EQUIPMENTS', whereClause );    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlSteamGenerator`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the CONTROLS table'
BEGIN

    DELETE FROM 
        CONTROLS
    WHERE
        CONTROL_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentAttribute`
(
IN equipmentId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EQUIPMENTS_ATTRIBUTES table'
BEGIN

    DELETE FROM
        EQUIPMENTS_ATTRIBUTES
    WHERE
        EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID = equipmentId
    AND
        EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteEquipmentTrackingsAttributes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the EQUIPMENT_TRACKINGS_ATTRIBUTES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'EQUIPMENT_TRACKINGS_ATTRIBUTES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteInspectionSelectedControl`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SELECTED_CONTROLS table matching the whereClause'
BEGIN
	
	CALL SpDeleteRecords( 'SELECTED_CONTROLS', whereClause );    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTubesZone`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TUBES_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'TUBES_ZONES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesEquipmentSpecific`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_SPECIFIC_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_SPECIFIC_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesEquipmentState`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_STATE_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_STATE_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesEquipmentStatistic`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_STATISTIC_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_STATISTIC_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesEquipmentTracking`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_TRACKING_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_TRACKING_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesEquipmentType`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_TYPE_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_TYPE_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlAttributes`
(
IN controlId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROLS_ATTRIBUTES table matching the where clause.'
BEGIN

    SELECT
        CA.CONTROL_ATTRIBUTE_CONTROL_ID AS CONTROL_ID,
        CA.CONTROL_ATTRIBUTE_ATTRIBUTE_ID AS ID,
        CA.CONTROL_ATTRIBUTE_VALUE AS VALUE,
        CA.CONTROL_ATTRIBUTE_CREATION_DATE AS VALUE_CREATION_DATE,
        CA.CONTROL_ATTRIBUTE_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE
    FROM
        CONTROLS_ATTRIBUTES CA
	WHERE  
		CA.CONTROL_ATTRIBUTE_CONTROL_ID = controlId;                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlSelectedEquipments`
(
IN controlId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SELECTED_EQUIPMENTS table matching the where clause.'
BEGIN

    SELECT
        SE.SELECTED_EQUIPMENT_CONTROL_ID AS CONTROL_ID,
        SE.SELECTED_EQUIPMENT_EQUIPMENT_ID AS EQUIPMENT_ID,
		E.EQUIPMENT_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
		ET.EQUIPMENT_TYPE_CATEGORY_ID AS CATEGORY_ID,
        SE.SELECTED_EQUIPMENT_ACQUISITION_LINE_ID AS ACQUISITION_LINE_ID,
		SE.SELECTED_EQUIPMENT_CREATION_DATE AS CREATION_DATE,
        SE.SELECTED_EQUIPMENT_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        SELECTED_EQUIPMENTS SE
			JOIN ACQUISITION_LINES AL ON AL.ACQUISITION_LINE_ID = SE.SELECTED_EQUIPMENT_ACQUISITION_LINE_ID
			JOIN EQUIPMENTS E ON E.EQUIPMENT_ID = SE.SELECTED_EQUIPMENT_EQUIPMENT_ID
			JOIN EQUIPMENT_TYPES ET ON ET.EQUIPMENT_TYPE_ID = E.EQUIPMENT_EQUIPMENT_TYPE_ID
	WHERE  
		SE.SELECTED_EQUIPMENT_CONTROL_ID = controlId
	ORDER BY
        AL.ACQUISITION_LINE_NUMBER ASC;                  
	                     
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlSteamGenerators`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROLS table for a Steam Generator organ type matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            C.CONTROL_ID AS ID,
                            C.CONTROL_LINKED_ID AS LINKED_ID,
                            C.CONTROL_INSPECTION_ID AS INSPECTION_ID,
                            C.CONTROL_ORGAN_ID AS ORGAN_ID,
							C.CONTROL_TECHNIQUE_ID AS TECHNIQUE_ID,
                            C.CONTROL_SECTOR_ID AS SECTOR_ID,
                            C.CONTROL_LEG_TYPE AS LEG_TYPE,
                            C.CONTROL_BEGIN_ZONE_ID AS BEGIN_ZONE_ID,
                            C.CONTROL_END_ZONE_ID AS END_ZONE_ID,
                            C.CONTROL_STATE AS STATE,
                            C.CONTROL_MODE AS MODE,
                            C.CONTROL_OPTION AS CONTROL_OPTION,
                            C.CONTROL_NAME AS NAME,
                            C.CONTROL_ORDER AS ORDER_NUMBER,
                            C.CONTROL_PROCEDURE1 AS PROCEDURE1,
                            C.CONTROL_PROCEDURE2 AS PROCEDURE2,
                            C.CONTROL_START_DATE AS START_DATE,
                            C.CONTROL_FINISH_DATE AS FINISH_DATE,
                            C.CONTROL_COMMENT AS COMMENT,
                            C.CONTROL_DATA_FORMAT AS DATA_FORMAT,
                            C.CONTROL_BEGIN_OFFSET AS BEGIN_OFFSET,
                            C.CONTROL_END_OFFSET AS END_OFFSET,
							C.CONTROL_SAVE_ALL_DATA AS SAVE_ALL_DATA,
                            C.CONTROL_CREATION_DATE AS CREATION_DATE,
                            C.CONTROL_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            CONTROLS C
								INNER JOIN ORGANS O ON C.CONTROL_ORGAN_ID = O.ORGAN_ID'
                    ,CONCAT(whereClause, ' AND O.ORGAN_TYPE = 0') );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetCreationDateFromLinkedTable`
(
tableName VARCHAR(200),
columnPrefixe VARCHAR(200),
whereClause VARCHAR(500),
OUT creationDate DATETIME)
SQL SECURITY DEFINER
BEGIN

    SET @strQuery = CONCAT( 'SELECT ', columnPrefixe, '_CREATION_DATE INTO @tmp FROM ', tableName , whereClause);

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    
    SET creationDate = @tmp;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentsAttributes`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENTS_ATTRIBUTES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            EA.EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID AS EQUIPMENT_ID,
                            EA.EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID AS ID,
                            EA.EQUIPMENT_ATTRIBUTE_VALUE AS VALUE,
                            EA.EQUIPMENT_ATTRIBUTE_CREATION_DATE AS VALUE_CREATION_DATE,
                            EA.EQUIPMENT_ATTRIBUTE_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE,
                            A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
                            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
                            A.ATTRIBUTE_NOT_NULL AS NOT_NULL,
                            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
                            A.ATTRIBUTE_VALUE1 AS VALUE1,
                            A.ATTRIBUTE_VALUE2 AS VALUE2,
                            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            EQUIPMENTS_ATTRIBUTES EA
                                 INNER JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = EA.EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentSpecificAttributes`
(
IN equipmentId BIGINT)
SQL SECURITY DEFINER
BEGIN
    CALL SpGetEquipmentsAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_SPECIFIC_ATTRIBUTES\' ) AND EA.EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID =',  equipmentId ) );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentStateAttributes`
(
IN equipmentId BIGINT)
SQL SECURITY DEFINER
BEGIN
    CALL SpGetEquipmentsAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_STATE_ATTRIBUTES\' ) AND EA.EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID =',  equipmentId ) );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentStatisticAttributes`
(
IN equipmentId BIGINT)
SQL SECURITY DEFINER
BEGIN
    CALL SpGetEquipmentsAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EQUIPMENT_STATISTIC_ATTRIBUTES\' ) AND EA.EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID =',  equipmentId ) );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentTrackingAttributes`
(
IN equipmentTrackingId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENT_TRACKINGS_ATTRIBUTES table matching the where clause.'
BEGIN
	
	SELECT
        ETA.ETA_EQUIPMENT_TRACKING_ID AS EQUIPMENT_TRACKING_ID,
        ETA.ETA_ATTRIBUTE_ID AS ID,
        ETA.ETA_VALUE AS VALUE,
        ETA.ETA_CREATION_DATE AS VALUE_CREATION_DATE,
        ETA.ETA_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE,
        A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
        A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
        A.ATTRIBUTE_NAME AS NAME,
        A.ATTRIBUTE_LABEL AS LABEL,
        A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
        A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
        A.ATTRIBUTE_NOT_NULL AS NOT_NULL,
        A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
        A.ATTRIBUTE_VALUE1 AS VALUE1,
        A.ATTRIBUTE_VALUE2 AS VALUE2,
        A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
        A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
        A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        EQUIPMENT_TRACKINGS_ATTRIBUTES ETA
                INNER JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = ETA.ETA_ATTRIBUTE_ID
    WHERE  ETA.ETA_EQUIPMENT_TRACKING_ID = equipmentTrackingId;                

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentTypeAttributes`
(
IN equipmentTypeId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENT_TYPES_ATTRIBUTES table matching the where clause.'
BEGIN
    SELECT
        ETA.ETA_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
        ETA.ETA_ATTRIBUTE_ID AS ID,
        ETA.ETA_VALUE AS VALUE,
        ETA.ETA_CREATION_DATE AS VALUE_CREATION_DATE,
        ETA.ETA_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE,
        A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
        A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
        A.ATTRIBUTE_NAME AS NAME,
        A.ATTRIBUTE_LABEL AS LABEL,
        A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
        A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
        A.ATTRIBUTE_NOT_NULL AS NOT_NULL,
        A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
        A.ATTRIBUTE_VALUE1 AS VALUE1,
        A.ATTRIBUTE_VALUE2 AS VALUE2,
        A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
        A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
        A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        EQUIPMENT_TYPES_ATTRIBUTES ETA
                INNER JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = ETA.ETA_ATTRIBUTE_ID
    WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( 'EQUIPMENT_TYPE_ATTRIBUTES' )
    AND ETA.ETA_EQUIPMENT_TYPE_ID = equipmentTypeId;                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEventsFiltered`
(
IN whereClause VARCHAR(500)
)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EVENTS table matching the from and where clause.'
BEGIN
    	
    CALL SpGetRecords( 'SELECT
                            SQL_CALC_FOUND_ROWS *,
                            E.EVENT_ID AS ID,
                            E.EVENT_CONTROL_ID AS CONTROL_ID,
                            E.EVENT_ACQUISITION_GROUP_ID AS ACQUISITION_GROUP_ID,
                            E.EVENT_LEVEL AS LEVEL,
                            E.EVENT_TYPE AS TYPE,
                            E.EVENT_LINE_NUMBER AS LINE_NUMBER,
                            E.EVENT_CODE AS CODE,
                            E.EVENT_SOURCE AS SOURCE,
                            E.EVENT_DESCRIPTION AS DESCRIPTION,
                            E.EVENT_ACQUIT_DATE AS ACQUIT_DATE,
                            E.EVENT_DETAIL1 AS DETAIL1,
                            E.EVENT_DETAIL2 AS DETAIL2,
                            E.EVENT_CREATION_DATE AS CREATION_DATE,
                            E.EVENT_MODIFICATION_DATE AS MODIFICATION_DATE'
                    , whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetInspectionAttributes`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the INSPECTIONS_ATTRIBUTES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            IA.INSPECTION_ATTRIBUTE_INSPECTION_ID AS INSPECTION_ID,
							IA.INSPECTION_ATTRIBUTE_ATTRIBUTE_ID AS ID,
							IA.INSPECTION_ATTRIBUTE_VALUE AS VALUE,
							IA.INSPECTION_ATTRIBUTE_CREATION_DATE AS VALUE_CREATION_DATE,
							IA.INSPECTION_ATTRIBUTE_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE,
							A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
                            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
                            A.ATTRIBUTE_NOT_NULL AS NOT_NULL,
                            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
                            A.ATTRIBUTE_VALUE1 AS VALUE1,
                            A.ATTRIBUTE_VALUE2 AS VALUE2,
                            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE                         
                        FROM
                            INSPECTIONS_ATTRIBUTES IA
                        INNER JOIN 
							ATTRIBUTES A ON A.ATTRIBUTE_ID = IA.INSPECTION_ATTRIBUTE_ATTRIBUTE_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetInspectionSelectedControls`
(
IN inspectionId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SELECTED_CONTROLS table matching the where clause.'
BEGIN

    SELECT
        SC.SELECTED_CONTROL_INSPECTION_ID AS INSPECTION_ID,
        SC.SELECTED_CONTROL_CONTROL_ID AS CONTROL_ID,
        SC.SELECTED_CONTROL_ACQUISITION_LINE_ID AS ACQUISITION_LINE_ID,
		SC.SELECTED_CONTROL_HOST AS HOST,
        SC.SELECTED_CONTROL_CREATION_DATE AS CREATION_DATE,
        SC.SELECTED_CONTROL_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        SELECTED_CONTROLS SC
	WHERE  
		SC.SELECTED_CONTROL_INSPECTION_ID = inspectionId;                     
	                     
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetModificationDateFromLinkedTable`
(
tableName VARCHAR(200),
columnPrefixe VARCHAR(200),
whereClause VARCHAR(500),
OUT modificationDate DATETIME)
SQL SECURITY DEFINER
BEGIN

    SET @strQuery = CONCAT( 'SELECT ', columnPrefixe, '_MODIFICATION_DATE INTO @tmp FROM ', tableName , whereClause);

    PREPARE stmt FROM @strQuery;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    
    SET modificationDate = @tmp;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetMessages`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the MESSAGES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            M.MESSAGE_ID AS ID,
                            M.MESSAGE_ACQUISITION_GROUP_ID AS ACQUISITION_GROUP_ID,
                            M.MESSAGE_ACQUISITION_ID AS ACQUISITION_ID,
                            M.MESSAGE_NAME AS NAME,
                            M.MESSAGE_TEXT AS TEXT,
                            M.MESSAGE_USER_NAME AS USER_NAME,
                            M.MESSAGE_CREATION_DATE AS CREATION_DATE,
                            M.MESSAGE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            MESSAGES M'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetPokActions`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the POK_ACTIONS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            PA.POK_ACTION_ID AS ID,
                            PA.POK_ACTION_ACQUISITION_GROUP_ID AS ACQUISITION_GROUP_ID,
                            PA.POK_ACTION_TYPE_ID AS TYPE_ID,
                            PA.POK_ACTION_REASON_ID AS REASON_ID,
                            T.ATTRIBUTE_NAME AS TYPE,
                            R.ATTRIBUTE_NAME AS REASON,
                            PA.POK_ACTION_CREATION_DATE AS CREATION_DATE,
                            PA.POK_ACTION_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            POK_ACTIONS PA
                                LEFT JOIN ATTRIBUTES T ON PA.POK_ACTION_TYPE_ID = T.ATTRIBUTE_ID 
                                LEFT JOIN ATTRIBUTES R ON PA.POK_ACTION_REASON_ID = R.ATTRIBUTE_ID '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetQualityCheckResults`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the QUALITY_CHECK_RESULTS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            QCR.QUALITY_CHECK_RESULT_ID AS ID,
                            QCR.QUALITY_CHECK_RESULT_ACQUISITION_FILE_ID AS ACQUISITION_FILE_ID,
                            QCR.QUALITY_CHECK_RESULT_CODE AS CODE,
                            QCR.QUALITY_CHECK_RESULT_LABEL AS LABEL,
                            QCR.QUALITY_CHECK_RESULT_IS_VQ AS IS_VQ,
                            QCR.QUALITY_CHECK_RESULT_CREATION_DATE AS CREATION_DATE,
                            QCR.QUALITY_CHECK_RESULT_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            QUALITY_CHECK_RESULTS QCR'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetQualityCheckResultValues`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the QUALITY_CHECK_RESULT_VALUES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            QCR.QUALITY_CHECK_RESULT_VALUE_ID AS ID,
                            QCR.QUALITY_CHECK_RESULT_VALUE_QUALITY_CHECK_RESULT_ID AS QUALITY_CHECK_RESULT_ID,
                            QCR.QUALITY_CHECK_RESULT_VALUE_DATA_TYPE AS DATA_TYPE,
                            QCR.QUALITY_CHECK_RESULT_VALUE_UNIT_ID AS UNIT_ID,
                            QCR.QUALITY_CHECK_RESULT_VALUE_VALUE AS VALUE,
                            QCR.QUALITY_CHECK_RESULT_VALUE_VALUE_MIN AS VALUE_MIN,
                            QCR.QUALITY_CHECK_RESULT_VALUE_VALUE_MAX AS VALUE_MAX,
                            QCR.QUALITY_CHECK_RESULT_VALUE_DEVIATION AS DEVIATION,
                            QCR.QUALITY_CHECK_RESULT_VALUE_CREATION_DATE AS CREATION_DATE,
                            QCR.QUALITY_CHECK_RESULT_VALUE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            QUALITY_CHECK_RESULT_VALUES QCR '
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateQualityCheckResult`
(
INOUT id BIGINT,
IN acquisitionFileId BIGINT,
IN code VARCHAR(10),
IN label VARCHAR(255),
IN isVq Bool,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the QUALITY_CHECK_RESULTS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO QUALITY_CHECK_RESULTS 
        (
            QUALITY_CHECK_RESULT_ACQUISITION_FILE_ID,
            QUALITY_CHECK_RESULT_CODE,
            QUALITY_CHECK_RESULT_LABEL,
            QUALITY_CHECK_RESULT_IS_VQ,
            QUALITY_CHECK_RESULT_CREATION_DATE
        )
        VALUES
        (
            acquisitionFileId,
            code,
            label,
            isVq,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('QUALITY_CHECK_RESULTS', 'QUALITY_CHECK_RESULT',  id, date );

    ELSE
    
        IF NOT FcQcResultExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'QUALITY_CHECK_RESULTS', id );
        END IF;

        UPDATE 
            QUALITY_CHECK_RESULTS 
        SET
            QUALITY_CHECK_RESULT_ACQUISITION_FILE_ID = acquisitionFileId,
            QUALITY_CHECK_RESULT_CODE = code,
            QUALITY_CHECK_RESULT_LABEL = label,
            QUALITY_CHECK_RESULT_IS_VQ = isVq,
            QUALITY_CHECK_RESULT_MODIFICATION_DATE = NOW()
        WHERE
            QUALITY_CHECK_RESULT_ID = id;
            
        CALL SpGetModificationDate('QUALITY_CHECK_RESULTS', 'QUALITY_CHECK_RESULT',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteQualityCheckResult`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the QUALITY_CHECK_RESULTS table'
BEGIN

    DELETE FROM 
       QUALITY_CHECK_RESULTS
    WHERE
        QUALITY_CHECK_RESULT_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteQualityCheckResults`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the QUALITY_CHECK_RESULTS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'QUALITY_CHECK_RESULTS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteQualityCheckResultValue`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the QUALITY_CHECK_RESULT_VALUES table'
BEGIN

    DELETE FROM 
        QUALITY_CHECK_RESULT_VALUES
    WHERE
        QUALITY_CHECK_RESULT_VALUE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteQualityCheckResultValues`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the QUALITY_CHECK_RESULT_VALUES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'QUALITY_CHECK_RESULT_VALUES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateMessage`
(
INOUT id BIGINT,
IN acquisitionGroupId BIGINT,
IN acquisitionId BIGINT,
IN name Varchar(255),
IN messageText Text,
IN userName Varchar(200),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the MESSAGES table or update an existing record'
BEGIN

    IF acquisitionId <= 0 THEN
        SET acquisitionId = NULL;
    END IF;

    IF id IS NULL THEN

        INSERT INTO MESSAGES 
        (
            MESSAGE_ACQUISITION_GROUP_ID,
            MESSAGE_ACQUISITION_ID,
            MESSAGE_NAME,
            MESSAGE_TEXT,
            MESSAGE_USER_NAME,
            MESSAGE_CREATION_DATE
        )
        VALUES
        (
            acquisitionGroupId,
            acquisitionId,
            name,
            messageText,
            userName,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('MESSAGES', 'MESSAGE',  id, date );

    ELSE
    
        IF NOT FcMessageExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'MESSAGES', id );
        END IF;

        UPDATE 
            MESSAGES 
        SET
            MESSAGE_ACQUISITION_GROUP_ID = acquisitionGroupId,
            MESSAGE_ACQUISITION_ID = acquisitionId,
            MESSAGE_NAME = name,
            MESSAGE_TEXT = messageText,
            MESSAGE_USER_NAME = userName,
            MESSAGE_MODIFICATION_DATE = NOW()
        WHERE
            MESSAGE_ID = id;
            
        CALL SpGetModificationDate('MESSAGES', 'MESSAGE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdatePokAction`
(
INOUT id BIGINT,
IN acquisitionGroupId BIGINT,
IN pokType Varchar(255),
IN pokReason Varchar(255),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the POK_ACTIONS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO POK_ACTIONS 
        (
            POK_ACTION_ACQUISITION_GROUP_ID,
            POK_ACTION_TYPE_ID,
            POK_ACTION_REASON_ID,
            POK_ACTION_CREATION_DATE
        )
        VALUES
        (
            acquisitionGroupId,
            FcGetAttributeId( CONCAT('POK_TYPE.',pokType)),
            FcGetAttributeId( CONCAT('POK_REASON.',pokReason)),
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('POK_ACTIONS', 'POK_ACTION',  id, date );

    ELSE
    
        IF NOT FcPokActionExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'POK_ACTION', id );
        END IF;

        UPDATE 
            POK_ACTIONS 
        SET
            POK_ACTION_ACQUISITION_GROUP_ID = acquisitionGroupId,
            POK_ACTION_TYPE_ID = FcGetAttributeId( CONCAT('POK_TYPE.',pokType)),
            POK_ACTION_REASON_ID = FcGetAttributeId( CONCAT('POK_REASON.',pokReason)),
            POK_ACTION_MODIFICATION_DATE = NOW()
        WHERE
            POK_ACTION_ID = id;
            
        CALL SpGetModificationDate('POK_ACTIONS', 'POK_ACTION',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateQualityCheckResultValue`
(
INOUT id BIGINT,
IN qcResultId BIGINT,
IN dataType Smallint,
IN unitId BIGINT,
IN value VARCHAR(20),
IN valueMin VARCHAR(20),
IN valueMax VARCHAR(20),
IN deviation VARCHAR(20),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the QUALITY_CHECK_RESULT_VALUES table or update an existing record'
BEGIN

    IF unitId <= 0 THEN
        SET unitId = NULL;
    END IF;

    IF id IS NULL THEN

        INSERT INTO QUALITY_CHECK_RESULT_VALUES 
        (
            QUALITY_CHECK_RESULT_VALUE_DATA_TYPE,
            QUALITY_CHECK_RESULT_VALUE_QUALITY_CHECK_RESULT_ID,
            QUALITY_CHECK_RESULT_VALUE_UNIT_ID,
            QUALITY_CHECK_RESULT_VALUE_VALUE,
            QUALITY_CHECK_RESULT_VALUE_VALUE_MIN,
            QUALITY_CHECK_RESULT_VALUE_VALUE_MAX,
            QUALITY_CHECK_RESULT_VALUE_DEVIATION,
            QUALITY_CHECK_RESULT_VALUE_CREATION_DATE
        )
        VALUES
        (
            dataType,
            qcResultId,
            unitId,
            value,
            valueMin,
            valueMax,
            deviation,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('QUALITY_CHECK_RESULT_VALUES', 'QUALITY_CHECK_RESULT_VALUE',  id, date );

    ELSE
    
        IF NOT FcQcResultValueExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'QUALITY_CHECK_RESULT_VALUES', id );
        END IF;

        UPDATE 
            QUALITY_CHECK_RESULT_VALUES 
        SET
            QUALITY_CHECK_RESULT_VALUE_DATA_TYPE = dataType,
            QUALITY_CHECK_RESULT_VALUE_QUALITY_CHECK_RESULT_ID = qcResultId,
            QUALITY_CHECK_RESULT_VALUE_UNIT_ID = unitId,
            QUALITY_CHECK_RESULT_VALUE_VALUE = value,
            QUALITY_CHECK_RESULT_VALUE_VALUE_MIN = valueMin,
            QUALITY_CHECK_RESULT_VALUE_VALUE_MAX = valueMax,
            QUALITY_CHECK_RESULT_VALUE_DEVIATION = deviation,
            QUALITY_CHECK_RESULT_VALUE_MODIFICATION_DATE = NOW()
        WHERE
            QUALITY_CHECK_RESULT_VALUE_ID = id;
            
        CALL SpGetModificationDate('QUALITY_CHECK_RESULT_VALUES', 'QUALITY_CHECK_RESULT_VALUE',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearQualityCheckResultsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the QUALITY_CHECK_RESULTS table'
BEGIN

    CALL SpDeleteQualityCheckResults( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearQualityCheckResultValuesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the QUALITY_CHECK_RESULT_VALUES table'
BEGIN

    CALL SpDeleteQualityCheckResultValues( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteInspectionAttribute`
(
IN inspectionId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the INSPECTIONS_ATTRIBUTES table'
BEGIN

    DELETE FROM
        INSPECTIONS_ATTRIBUTES
    WHERE
        INSPECTION_ATTRIBUTE_INSPECTION_ID = inspectionId
	AND
		INSPECTION_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionEquipments`
(
IN acquisitionId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITIONS_EQUIPMENTS and EQUIPMENTS tables matching the acquisition id.'
BEGIN

        SELECT
            AE.ACQUISITION_EQUIPMENT_EQUIPMENT_ID AS ID,
            E.EQUIPMENT_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
            E.EQUIPMENT_CONDITION AS ECONDITION,
            E.EQUIPMENT_FME_STATUS AS FME_STATUS,
            E.EQUIPMENT_STATE AS STATE,
            E.EQUIPMENT_SERIAL_NUMBER AS SERIAL_NUMBER,
            E.EQUIPMENT_NAME AS NAME,
            E.EQUIPMENT_CONNECTION_STRING AS CONNECTION_STRING,
            E.EQUIPMENT_MODULE_CONNECTION_STRING AS MODULE_CONNECTION_STRING,
            E.EQUIPMENT_VERSION AS VERSION,
            E.EQUIPMENT_IS_TEMPLATE AS IS_TEMPLATE,
            E.EQUIPMENT_CUSTOM_ATTRIBUTES_LAYOUT_FILE AS CUSTOM_ATTRIBUTES_LAYOUT_FILE,
            E.EQUIPMENT_COMMENT AS COMMENT,
            E.EQUIPMENT_CREATION_DATE AS CREATION_DATE,
            E.EQUIPMENT_MODIFICATION_DATE AS MODIFICATION_DATE
        FROM
            ACQUISITIONS_EQUIPMENTS AE
                JOIN EQUIPMENTS E ON AE.ACQUISITION_EQUIPMENT_EQUIPMENT_ID = E.EQUIPMENT_ID
        WHERE 
                AE.ACQUISITION_EQUIPMENT_ACQUISITION_ID = acquisitionId;                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAcquisitionGroupLines`
( IN acquisitionGroupId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACQUISITION_GROUPS_LINES table for the given acquisitionGroupId.'
BEGIN
    SELECT
        AGL.ACQUISITION_LINE_ID AS ID,
		AL.ACQUISITION_LINE_NUMBER AS NUMBER,
		AL.ACQUISITION_LINE_LABEL AS LABEL,
		AL.ACQUISITION_LINE_COLOR AS COLOR,
		AL.ACQUISITION_LINE_CREATION_DATE AS CREATION_DATE,
		AL.ACQUISITION_LINE_MODIFICATION_DATE AS MODIFICATION_DATE 
    FROM
        ACQUISITION_GROUPS_LINES AGL
           INNER JOIN ACQUISITION_LINES AL ON AL.ACQUISITION_LINE_ID = AGL.ACQUISITION_LINE_ID
    WHERE  AGL.ACQUISITION_GROUP_ID = acquisitionGroupId;      

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesControl`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'CONTROLS_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'CONTROLS_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesControlSteamGenerator`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	CALL SpGetAttributesControl ( CONCAT( 'OR A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'CONTROL_STEAM_GENERATORS_ATTRIBUTES\' )', ' ', whereClause  ) );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTubesByControlZone`
(
IN controlZoneId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROL_ZONES_TUBES table matching the controlZoneId.'
BEGIN

SELECT
        CTZ.CONTROL_ZONE_TUBE_TUBE_ID AS TUBE_ID
    FROM
        CONTROL_ZONES_TUBES CTZ
    WHERE
        CTZ.CONTROL_ZONE_TUBE_ZONE_ID = controlZoneId;                     

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateBackupHistory`
(
INOUT id BIGINT,
IN databaseVersion VARCHAR(20),
IN file VARCHAR(255),
IN comment TEXT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (BACKUP_HISTORIES) table'
BEGIN

    IF id IS NULL THEN

		INSERT INTO BACKUP_HISTORIES
		(
			BACKUP_HISTORY_DATABASE_VERSION,
			BACKUP_HISTORY_FILE,
			BACKUP_HISTORY_COMMENT,
            BACKUP_HISTORY_CREATION_DATE
		)
		VALUES
		(
			databaseVersion,
			file,
			comment,
            NOW()
		);
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('BACKUP_HISTORIES', 'BACKUP_HISTORY',  id, date );

    ELSE
    
        IF NOT FcBackupHistoryExist( id ) THEN
            CALL SpRaiseRecordNotFound( 'BACKUP_HISTORIES', id );
        END IF;

        UPDATE 
            BACKUP_HISTORIES 
        SET
			BACKUP_HISTORY_DATABASE_VERSION = databaseVersion,
			BACKUP_HISTORY_FILE = file,
			BACKUP_HISTORY_COMMENT = comment,
            BACKUP_HISTORY_MODIFICATION_DATE = NOW()
        WHERE
            BACKUP_HISTORY_ID = id;
            
        CALL SpGetModificationDate('BACKUP_HISTORIES', 'BACKUP_HISTORY',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateDatabaseVersion`
(
INOUT id BIGINT,
IN value Varchar(20),
IN author VARCHAR(50),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the DATABASE_VERSIONS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO DATABASE_VERSIONS 
        (
            DATABASE_VERSION_VALUE,
            DATABASE_VERSION_AUTHOR,
            DATABASE_VERSION_CREATION_DATE
        )
        VALUES
        (
            value,
            author,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('DATABASE_VERSIONS', 'DATABASE_VERSION',  id, date );

    ELSE
    
        IF NOT FcDatabaseVersionExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'DATABASE_VERSIONS', id );
        END IF;

        UPDATE 
            DATABASE_VERSIONS 
        SET
            DATABASE_VERSION_VALUE = value,
            DATABASE_VERSION_AUTHOR = author,
            DATABASE_VERSION_MODIFICATION_DATE = NOW()
        WHERE
            DATABASE_VERSION_ID = id;
            
        CALL SpGetModificationDate('DATABASE_VERSIONS', 'DATABASE_VERSION',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateInspectionAttribute`
(
IN inspectionId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the INSPECTIONS_ATTRIBUTES table or update an existing record'
BEGIN

	SET date = NOW();
    IF FcInspectionAttributeExist( inspectionId, attributeId ) = FALSE THEN
 		
        INSERT INTO INSPECTIONS_ATTRIBUTES 
        (
            INSPECTION_ATTRIBUTE_INSPECTION_ID,
            INSPECTION_ATTRIBUTE_ATTRIBUTE_ID,
            INSPECTION_ATTRIBUTE_VALUE,
            INSPECTION_ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            inspectionId,
            attributeId,
            value,
            date
        );
     
    ELSE
    
        UPDATE
            INSPECTIONS_ATTRIBUTES 
        SET
            INSPECTION_ATTRIBUTE_VALUE = value,
            INSPECTION_ATTRIBUTE_MODIFICATION_DATE = date
        WHERE
            INSPECTION_ATTRIBUTE_INSPECTION_ID = inspectionId
        AND
            INSPECTION_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateQueueFileTransfer`
(
INOUT id BIGINT,
IN controlId BIGINT,
IN status SMALLINT,
IN source VARCHAR(255),
IN destination VARCHAR(255),
IN priority INT,

OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the QUEUE_FILES_TRANSFER table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO QUEUE_FILES_TRANSFER
        (
            QUEUE_FILE_TRANSFER_CONTROL_ID,
            QUEUE_FILE_TRANSFER_STATUS,
			QUEUE_FILE_TRANSFER_SOURCE,
			QUEUE_FILE_TRANSFER_DESTINATION,
			QUEUE_FILE_TRANSFER_PRIORITY,
            QUEUE_FILE_TRANSFER_CREATION_DATE
        )
        VALUES
        (
            controlId,
            status,
			source,
			destination,
			priority,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('QUEUE_FILES_TRANSFER', 'QUEUE_FILE_TRANSFER',  id, date );

    ELSE
    
        IF NOT FcQueueFileTransferExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'QUEUE_FILES_TRANSFER', id );
        END IF;

        UPDATE 
            QUEUE_FILES_TRANSFER 
        SET
            QUEUE_FILE_TRANSFER_CONTROL_ID = controlId,
            QUEUE_FILE_TRANSFER_STATUS = status,
			QUEUE_FILE_TRANSFER_SOURCE = source,
			QUEUE_FILE_TRANSFER_DESTINATION = destination,
			QUEUE_FILE_TRANSFER_PRIORITY = priority,
            QUEUE_FILE_TRANSFER_MODIFICATION_DATE = NOW()
        WHERE
            QUEUE_FILE_TRANSFER_ID = id;
            
        CALL SpGetModificationDate('QUEUE_FILES_TRANSFER', 'QUEUE_FILE_TRANSFER',  id, date );

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTechniqueAttribute`
(
IN techniqueId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TECHNIQUES_ATTRIBUTES table or update an existing record'
BEGIN

	SET date = NOW();
    IF FcTechniqueAttributeExist( techniqueId, attributeId ) = FALSE THEN
 		
        INSERT INTO TECHNIQUES_ATTRIBUTES 
        (
            TECHNIQUE_ATTRIBUTE_TECHNIQUE_ID,
            TECHNIQUE_ATTRIBUTE_ATTRIBUTE_ID,
            TECHNIQUE_ATTRIBUTE_VALUE,
            TECHNIQUE_ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            techniqueId,
            attributeId,
            value,
            date
        );
     
    ELSE
    
        UPDATE
            TECHNIQUES_ATTRIBUTES 
        SET
            TECHNIQUE_ATTRIBUTE_VALUE = value,
            TECHNIQUE_ATTRIBUTE_MODIFICATION_DATE = date
        WHERE
            TECHNIQUE_ATTRIBUTE_TECHNIQUE_ID = techniqueId
        AND
            TECHNIQUE_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateTechniqueEquipmentType`
(
IN techniqueId BIGINT,
IN equipmentTypeId BIGINT,
IN equipmentConfigurationId BIGINT,
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the TECHNIQUES_EQUIPMENT_TYPES table or update an existing record'
BEGIN

	SET date = NOW();
    IF FcTechniqueEquipmentTypeExist( techniqueId, equipmentTypeId ) = FALSE THEN
 		
        INSERT INTO TECHNIQUES_EQUIPMENT_TYPES
        (
            TET_TECHNIQUE_ID,
            TET_EQUIPMENT_TYPE_ID,
            TET_EQUIPMENT_CONFIGURATION_ID,
            TET_CREATION_DATE
        )
        VALUES
        (
            techniqueId,
            equipmentTypeId,
            equipmentConfigurationId,
            date
        );
     
    ELSE
    
        UPDATE
            TECHNIQUES_EQUIPMENT_TYPES 
        SET
            TET_EQUIPMENT_CONFIGURATION_ID = equipmentConfigurationId,
            TET_MODIFICATION_DATE = date
        WHERE
            TET_TECHNIQUE_ID = techniqueId
        AND
            TET_EQUIPMENT_TYPE_ID = equipmentTypeId;
    
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteBackupHistory`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the BACKUP_HISTORIES table'
BEGIN

    DELETE FROM
        BACKUP_HISTORIES
    WHERE
        BACKUP_HISTORY_ID = id;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteMessage`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the MESSAGES table'
BEGIN

    DELETE FROM 
        MESSAGES
    WHERE
        MESSAGE_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeletePokAction`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the POK_ACTIONS table'
BEGIN

    DELETE FROM 
        POK_ACTIONS
    WHERE
        POK_ACTION_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteQueueFileTransfer`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the QUEUE_FILES_TRANSFER table'
BEGIN

    DELETE FROM 
        QUEUE_FILES_TRANSFER
    WHERE
        QUEUE_FILE_TRANSFER_ID = id;
		    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTechniqueAttribute`
(
IN techniqueId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TECHNIQUES_ATTRIBUTES table matching the whereClause'
BEGIN

    DELETE FROM
		TECHNIQUES_ATTRIBUTES
	WHERE
		TECHNIQUE_ATTRIBUTE_TECHNIQUE_ID = techniqueId
	AND
		TECHNIQUE_ATTRIBUTE_ATTRIBUTE_ID = attributeId;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteTechniqueEquipmentType`
(
IN techniqueId BIGINT,
IN equipmentTypeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the TECHNIQUES_EQUIPMENT_TYPES table matching the whereClause'
BEGIN

    DELETE FROM
		TECHNIQUES_EQUIPMENT_TYPES
	WHERE
		TET_TECHNIQUE_ID = techniqueId
	AND
		TET_EQUIPMENT_TYPE_ID = equipmentTypeId;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetDatabaseVersions`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the DATABASE_VERSIONS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            DV.DATABASE_VERSION_ID AS ID,
                            DV.DATABASE_VERSION_VALUE AS VALUE,
                            DV.DATABASE_VERSION_AUTHOR AS AUTHOR,
                            DV.DATABASE_VERSION_CREATION_DATE AS CREATION_DATE,
                            DV.DATABASE_VERSION_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            DATABASE_VERSIONS DV'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetQueueFilesTransfer`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the QUEUE_FILES_TRANSFER table matching the where clause.'
BEGIN

	CALL SpGetRecords( 'SELECT
							Q.QUEUE_FILE_TRANSFER_ID AS ID,
							Q.QUEUE_FILE_TRANSFER_CONTROL_ID AS CONTROL_ID,
							Q.QUEUE_FILE_TRANSFER_STATUS AS STATUS,
							Q.QUEUE_FILE_TRANSFER_SOURCE AS SOURCE,
							Q.QUEUE_FILE_TRANSFER_DESTINATION AS DESTINATION,
							Q.QUEUE_FILE_TRANSFER_PRIORITY AS PRIORITY,
							Q.QUEUE_FILE_TRANSFER_CREATION_DATE AS CREATION_DATE,
							Q.QUEUE_FILE_TRANSFER_MODIFICATION_DATE AS MODIFICATION_DATE
                       FROM
							QUEUE_FILES_TRANSFER Q'
                    ,whereClause );   

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTechniqueAttributes`
(
IN techniqueId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TECHNIQUES_ATTRIBUTES table matching the where clause.'
BEGIN     
		
		SELECT
            TA.TECHNIQUE_ATTRIBUTE_TECHNIQUE_ID AS TECHNIQUE_ID,
            TA.TECHNIQUE_ATTRIBUTE_ATTRIBUTE_ID AS ID,
            TA.TECHNIQUE_ATTRIBUTE_VALUE AS VALUE,
            TA.TECHNIQUE_ATTRIBUTE_CREATION_DATE AS VALUE_CREATION_DATE,
            TA.TECHNIQUE_ATTRIBUTE_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE,
            A.ATTRIBUTE_UNIT_ID AS UNIT_ID,
            A.ATTRIBUTE_DATA_TYPE AS DATA_TYPE,
            A.ATTRIBUTE_NAME AS NAME,
            A.ATTRIBUTE_LABEL AS LABEL,
            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
            A.ATTRIBUTE_DESCRIPTION AS DESCRIPTION,
            A.ATTRIBUTE_NOT_NULL AS NOT_NULL,
            A.ATTRIBUTE_DEFAULT_VALUE AS DEFAULT_VALUE,
            A.ATTRIBUTE_VALUE1 AS VALUE1,
            A.ATTRIBUTE_VALUE2 AS VALUE2,
            A.ATTRIBUTE_VALUE_FORMAT AS VALUE_FORMAT,
            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
        FROM
            TECHNIQUES_ATTRIBUTES TA
                INNER JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = TA.TECHNIQUE_ATTRIBUTE_ATTRIBUTE_ID
        WHERE  
            TA.TECHNIQUE_ATTRIBUTE_TECHNIQUE_ID = techniqueId;                     

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetTechniqueEquipmentTypes`
(
IN techniqueId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the TECHNIQUES_EQUIPMENT_TYPES table matching the where clause.'
BEGIN     
		
		SELECT
            TET.TET_TECHNIQUE_ID AS TECHNIQUE_ID,
            TET.TET_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
            TET.TET_EQUIPMENT_CONFIGURATION_ID AS EQUIPMENT_CONFIGURATION_ID,
            TET.TET_CREATION_DATE AS CREATION_DATE,
            TET.TET_MODIFICATION_DATE AS MODIFICATION_DATE
        FROM
            TECHNIQUES_EQUIPMENT_TYPES TET
        WHERE  
            TET.TET_TECHNIQUE_ID = techniqueId;                     

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateMessageType`
(
INOUT id BIGINT,
IN name VARCHAR(30),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (MESSAGE_TYPE) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'MESSAGE_TYPE' ),
                               NULL,
                               NULL,
                               NULL,
                               name,
                               name,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdatePokReason`
(
INOUT id BIGINT,
IN name VARCHAR(30),
IN relatedId BIGINT,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (POK_REASON) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'POK_REASON' ),
                               relatedId,
                               NULL,
                               NULL,
                               name,
                               name,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdatePokType`
(
INOUT id BIGINT,
IN name VARCHAR(30),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (POK_TYPE) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'POK_TYPE' ),
                               NULL,
                               NULL,
                               NULL,
                               name,
                               name,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteDatabaseVersion`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the DATABASE_VERSIONS table'
BEGIN

    DELETE FROM 
        DATABASE_VERSIONS
    WHERE
        DATABASE_VERSION_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteMessageType`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeletePokReason`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeletePokType`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetMessageTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
						 WHERE A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'MESSAGE_TYPE\' )'
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetPokReasons`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_RELATED_ID AS RELATED_ID,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
                         WHERE A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'POK_REASON\' )'
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetPokTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
						 WHERE A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'POK_TYPE\' )'
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUnitsInUnitConfiguration`
(
IN unitConfigurationId BIGINT,
IN unitId BIGINT,
IN decimals INT,
IN dimension INT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the UNITS_IN_UNIT_CONFIGURATIONS table'
BEGIN

    INSERT INTO UNITS_IN_UNIT_CONFIGURATIONS 
    (
        UNIT_CONFIGURATION_ID,
        UNIT_ID,
        UNIT_DECIMALS,
        UNIT_DIMENSION
    )
    VALUES
    (
        unitConfigurationId,
        unitId,
        decimals,
		dimension
    );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateActor`
(
INOUT id BIGINT,
IN actorType SMALLINT,
IN name VARCHAR(50),
IN location VARCHAR(255),
IN phoneNumber VARCHAR(50),
IN logo BLOB,
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ACTORS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO ACTORS 
        (
			ACTOR_TYPE,
            ACTOR_NAME,
            ACTOR_LOCATION,
            ACTOR_PHONE_NUMBER,
            ACTOR_LOGO,
            ACTOR_CREATION_DATE
        )
        VALUES
        (
            actorType,
            name,
            location,
            phoneNumber,
            logo,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('ACTORS', 'ACTOR',  id, date );

    ELSE
    
        IF NOT FcActorExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'ACTORS', id );
        END IF;

        UPDATE 
            ACTORS 
        SET
	    ACTOR_TYPE = actorType,
            ACTOR_NAME = name,
            ACTOR_LOCATION = location,
            ACTOR_PHONE_NUMBER = phoneNumber,
            ACTOR_LOGO = logo,
            ACTOR_MODIFICATION_DATE = NOW()
        WHERE
            ACTOR_ID = id;
            
        CALL SpGetModificationDate('ACTORS', 'ACTOR',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateUnitConfiguration`
(
INOUT id BIGINT,
IN name VARCHAR(255),
OUT date DATETIME)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the UNIT_CONFIGURATIONS table or update an existing record'
BEGIN

    IF id IS NULL THEN

        INSERT INTO UNIT_CONFIGURATIONS 
        (
            UNIT_CONFIGURATION_NAME,
            UNIT_CONFIGURATION_CREATION_DATE
        )
        VALUES
        (
            name,
            NOW()
        );
        
        SET id = LAST_INSERT_ID();
        
        CALL SpGetCreationDate('UNIT_CONFIGURATIONS', 'UNIT_CONFIGURATION',  id, date );

    ELSE
    
        IF NOT FcUnitConfigurationExist( id ) THEN
            CALL SpRaiseRecordNotFoundError( 'UNIT_CONFIGURATIONS', id );
        END IF;

        UPDATE 
            UNIT_CONFIGURATIONS 
        SET
            UNIT_CONFIGURATION_NAME = name,
            UNIT_CONFIGURATION_MODIFICATION_DATE = NOW()
        WHERE
            UNIT_CONFIGURATION_ID = id;
            
        CALL SpGetModificationDate('UNIT_CONFIGURATIONS', 'UNIT_CONFIGURATION',  id, date );

    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearActorsTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the ACTORS table'
BEGIN

    CALL SpDeleteActors( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteActor`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ACTORS table'
BEGIN

    DELETE FROM 
        ACTORS
    WHERE
        ACTOR_ID = id;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteActors`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the ACTORS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'ACTORS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUnitConfiguration`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the UNIT_CONFIGURATIONS table'
BEGIN

    DELETE FROM
        UNIT_CONFIGURATIONS
    WHERE
        UNIT_CONFIGURATION_ID = id;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUnitConfigurations`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the UNIT_CONFIGURATIONS table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'UNIT_CONFIGURATIONS', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteUnitsInUnitConfigurations`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the UNITS_IN_UNIT_CONFIGURATIONS table matching the whereClause'
BEGIN
	
	CALL SpDeleteRecords( 'UNITS_IN_UNIT_CONFIGURATIONS', whereClause );    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetActors`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the ACTORS table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            A.ACTOR_ID AS ID,
			    A.ACTOR_TYPE AS TYPE,
                            A.ACTOR_NAME AS NAME,
                            A.ACTOR_LOCATION AS LOCATION,
                            A.ACTOR_PHONE_NUMBER AS PHONE_NUMBER,
                            A.ACTOR_LOGO AS LOGO,
                            A.ACTOR_CREATION_DATE AS CREATION_DATE,
                            A.ACTOR_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            ACTORS A'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetUnitConfigurations`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

    CALL SpGetRecords( 'SELECT
                            UC.UNIT_CONFIGURATION_ID AS ID,
                            UC.UNIT_CONFIGURATION_NAME AS NAME,
                            UC.UNIT_CONFIGURATION_CREATION_DATE AS CREATION_DATE,
                            UC.UNIT_CONFIGURATION_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            UNIT_CONFIGURATIONS UC'
                    ,whereClause );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetUnitsInUnitConfigurations`
(
IN whereClause VARCHAR(500))
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the UNITS_IN_UNIT_CONFIGURATIONS table matching the whereClause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            UUC.UNIT_CONFIGURATION_ID AS UNIT_CONFIGURATION_ID,
                            UUC.UNIT_ID AS UNIT_ID,
                            UUC.UNIT_DECIMALS AS UNIT_DECIMALS,
                            UUC.UNIT_DIMENSION AS UNIT_DIMENSION
                        FROM
                            UNITS_IN_UNIT_CONFIGURATIONS UUC'
                    ,whereClause );                 
	                     
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateControlTube`
(
IN controlId BIGINT,
IN tubeId BIGINT,
IN isToControl TINYINT(1),
IN isDone TINYINT(1),
IN isObstructedInHotLeg TINYINT(1),
IN isObstructedInColdLeg TINYINT(1),
IN isPluggedInHotLeg TINYINT(1),
IN isHighPluggedInHotLeg TINYINT(1),
IN isPluggedInColdLeg TINYINT(1),
IN isHighPluggedInColdLeg TINYINT(1),
IN hasIndications TINYINT(1),
IN isCharacteristic TINYINT(1),
IN isUnplugged TINYINT(1),
OUT date DATETIME
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the CONTROLS_TUBES table or update an existing record'
BEGIN

    SET date = NOW();
    IF FcControlTubeExist( controlId, tubeId ) = FALSE THEN
 		
        INSERT INTO CONTROLS_TUBES 
        (
            CONTROL_TUBE_CONTROL_ID,
            CONTROL_TUBE_TUBE_ID,
            CONTROL_TUBE_IS_TO_CONTROL,
            CONTROL_TUBE_IS_DONE,
            CONTROL_TUBE_IS_OBSTRUCTED_IN_HOT_LEG,
            CONTROL_TUBE_IS_OBSTRUCTED_IN_COLD_LEG,
            CONTROL_TUBE_IS_PLUGGED_IN_HOT_LEG,
            CONTROL_TUBE_IS_HIGH_PLUGGED_IN_HOT_LEG,
            CONTROL_TUBE_IS_PLUGGED_IN_COLD_LEG,
            CONTROL_TUBE_IS_HIGH_PLUGGED_IN_COLD_LEG,
            CONTROL_TUBE_HAS_INDICATIONS,
            CONTROL_TUBE_IS_CHARACTERISTIC,
            CONTROL_TUBE_IS_UNPLUGGED,
            CONTROL_TUBE_CREATION_DATE
        )
        VALUES
        (
            controlId,
            tubeId,
            isToControl,
            isDone,
            isObstructedInHotLeg,
            isObstructedInColdLeg,
            isPluggedInHotLeg,
            isHighPluggedInHotLeg,
            isPluggedInColdLeg,
            isHighPluggedInColdLeg,
            hasIndications,
            isCharacteristic,
            isUnplugged,
            date
        );
     
    ELSE
    
        UPDATE
            CONTROLS_TUBES 
        SET
            CONTROL_TUBE_STATE = state,
            CONTROL_TUBE_IS_TO_CONTROL = isToControl,
            CONTROL_TUBE_IS_DONE = isDone,
            CONTROL_TUBE_IS_OBSTRUCTED_IN_HOT_LEG = isObstructedInHotLeg,
            CONTROL_TUBE_IS_OBSTRUCTED_IN_COLD_LEG = isObstructedInColdLeg,
            CONTROL_TUBE_IS_PLUGGED_IN_HOT_LEG = isPluggedInHotLeg,
            CONTROL_TUBE_IS_HIGH_PLUGGED_IN_HOT_LEG = isHighPluggedInHotLeg,
            CONTROL_TUBE_IS_PLUGGED_IN_COLD_LEG = isPluggedInColdLeg,
            CONTROL_TUBE_IS_HIGH_PLUGGED_IN_COLD_LEG = isHighPluggedInColdLeg,
            CONTROL_TUBE_HAS_INDICATIONS = hasIndications,
            CONTROL_TUBE_IS_CHARACTERISTIC = isCharacteristic,
            CONTROL_TUBE_IS_UNPLUGGED = isUnplugged,
            CONTROL_TUBE_MODIFICATION_DATE = date
        WHERE
            CONTROL_TUBE_CONTROL_ID = controlId
        AND
            CONTROL_TUBE_TUBE_ID = tubeId;
    
    END IF;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateExamAttribute`
(
IN examId BIGINT,
IN attributeId BIGINT,
IN value VARCHAR(255),
OUT date DATETIME
)
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the EXAMS_ATTRIBUTES table or update an existing record'
BEGIN

	SET date = NOW();
    IF FcExamAttributeExist( examId, attributeId ) = FALSE THEN
 		
        INSERT INTO EXAMS_ATTRIBUTES 
        (
            EXAM_ATTRIBUTE_EXAM_ID,
            EXAM_ATTRIBUTE_ATTRIBUTE_ID,
            EXAM_ATTRIBUTE_VALUE,
            EXAM_ATTRIBUTE_CREATION_DATE
        )
        VALUES
        (
            examId,
            attributeId,
            value,
            date
        );
     
    ELSE
    
        UPDATE
            EXAMS_ATTRIBUTES 
        SET
            EXAM_ATTRIBUTE_VALUE = value,
            EXAM_ATTRIBUTE_MODIFICATION_DATE = date
        WHERE
            EXAM_ATTRIBUTE_INSPECTION_ID = examId
        AND
            EXAM_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
    END IF;
        
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearControlsTubesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the CONTROLS_TUBES table'
BEGIN

    CALL SpDeleteControlsTubes( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlsTubes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the CONTROLS_TUBES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'CONTROLS_TUBES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteControlTube`
(
IN controlId BIGINT,
IN tubeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the CONTROLS_TUBES table'
BEGIN

    DELETE FROM 
        CONTROLS_TUBES
    WHERE
        CONTROL_TUBE_TUBE_ID = tubeId
    AND
        CONTROL_TUBE_CONTROL_ID = controlId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteExamAttribute`
(
IN examId BIGINT,
IN attributeId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the EXAMS_ATTRIBUTES table'
BEGIN

    DELETE FROM
        EXAMS_ATTRIBUTES
    WHERE
        EXAM_ATTRIBUTE_EXAM_ID = examId
	AND
		EXAM_ATTRIBUTE_ATTRIBUTE_ID = attributeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesExam`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EXAMS_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'EXAMS_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlsTubes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROLS_TUBES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            CT.CONTROL_TUBE_TUBE_ID AS TUBE_ID,
                            CT.CONTROL_TUBE_CONTROL_ID AS CONTROL_ID,
                            CT.CONTROL_TUBE_IS_TO_CONTROL AS IS_TO_CONTROL,
                            CT.CONTROL_TUBE_IS_DONE AS DONE,
                            CT.CONTROL_TUBE_IS_OBSTRUCTED_IN_HOT_LEG AS IS_OBSTRUCTED_IN_HOT_LEG,
                            CT.CONTROL_TUBE_IS_OBSTRUCTED_IN_COLD_LEG AS IS_OBSTRUCTED_IN_COLD_LEG,
                            CT.CONTROL_TUBE_IS_PLUGGED_IN_HOT_LEG AS IS_PLUGGED_IN_HOT_LEG,
                            CT.CONTROL_TUBE_IS_HIGH_PLUGGED_IN_HOT_LEG AS IS_HIGH_PLUGGED_IN_HOT_LEG,
                            CT.CONTROL_TUBE_IS_PLUGGED_IN_COLD_LEG AS IS_PLUGGED_IN_COLD_LEG,
                            CT.CONTROL_TUBE_IS_HIGH_PLUGGED_IN_COLD_LEG AS IS_HIGH_PLUGGED_IN_COLD_LEG,
                            CT.CONTROL_TUBE_HAS_INDICATIONS AS HAS_INDICATIONS,
                            CT.CONTROL_TUBE_IS_CHARACTERISTIC AS IS_CHARACTERISTIC,
                            CT.CONTROL_TUBE_IS_UNPLUGGED AS IS_UNPLUGGED,
                            C.CONTROL_NAME AS CONTROL_NAME,
                            T.TUBE_ROW_HOT_LEG AS ROW_HOT_LEG,
                            T.TUBE_COLUMN_HOT_LEG AS COLUMN_HOT_LEG,
                            T.TUBE_ROW_COLD_LEG AS ROW_COLD_LEG,
                            T.TUBE_COLUMN_COLD_LEG AS COLUMN_COLD_LEG,
                            CT.CONTROL_TUBE_CREATION_DATE AS CREATION_DATE,
                            CT.CONTROL_TUBE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            CONTROLS_TUBES CT
                                JOIN CONTROLS C ON CT.CONTROL_TUBE_CONTROL_ID = C.CONTROL_ID
                                JOIN TUBES T ON CT.CONTROL_TUBE_TUBE_ID = T.TUBE_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetControlsTubesByControl`
(
IN controlId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the CONTROLS_TUBES table matching the controlId.'
BEGIN

    CALL SpGetControlsTubes( CONCAT( 'WHERE CT.CONTROL_TUBE_CONTROL_ID = ', controlId ) );                   

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetEquipmentTypeFailureModes`
(
IN equipmentTypeId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EQUIPMENT_TYPES_ATTRIBUTES table matching the where clause.'
BEGIN
    SELECT
        ETA.ETA_EQUIPMENT_TYPE_ID AS EQUIPMENT_TYPE_ID,
        ETA.ETA_ATTRIBUTE_ID AS ID,
        A.ATTRIBUTE_NAME AS NAME,
        A.ATTRIBUTE_LABEL AS LABEL,
        A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
        A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
        A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
    FROM
        EQUIPMENT_TYPES_ATTRIBUTES ETA
                INNER JOIN ATTRIBUTES A ON A.ATTRIBUTE_ID = ETA.ETA_ATTRIBUTE_ID
    WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( 'REMOVAL_FAILURE_MODE' )
	AND ETA.ETA_EQUIPMENT_TYPE_ID = equipmentTypeId;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetExamAttributes`
(
IN examId BIGINT)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the EXAMS_ATTRIBUTES table matching the where clause.'
BEGIN

    SELECT
        EA.EXAM_ATTRIBUTE_INSPECTION_ID AS EXAM_ID,
        EA.EXAM_ATTRIBUTE_ATTRIBUTE_ID AS ID,
        EA.EXAM_ATTRIBUTE_VALUE AS VALUE,
        EA.EXAM_ATTRIBUTE_CREATION_DATE AS VALUE_CREATION_DATE,
        EA.EXAM_ATTRIBUTE_MODIFICATION_DATE AS VALUE_MODIFICATION_DATE
    FROM
        EXAMS_ATTRIBUTES EA
	WHERE  
		EA.EXAM_ATTRIBUTE_EXAM_ID = examId;                     
	                     
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetRemovalFailureModes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
                         WHERE A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'REMOVAL_FAILURE_MODE\' )'
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetRemovalFailureTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_RELATED_ID AS RELATED_ID,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
                         WHERE A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'REMOVAL_FAILURE_TYPE\' )'
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetRemovalReasons`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_RELATED_ID AS RELATED_ID,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
                         WHERE A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'REMOVAL_REASON\' )'
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesInspectionLabel`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'INSPECTION_LABEL_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'INSPECTION_LABEL_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetAttributesInspectionStatistic`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	IF whereClause IS NULL OR whereClause = '' THEN
		CALL SpGetAttributes( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'INSPECTION_STATISTIC_ATTRIBUTES\' )');
    ELSE
		CALL SpGetAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'INSPECTION_STATISTIC_ATTRIBUTES\' )', ' ', whereClause ) );
	END IF;
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetInspectionLabelAttributes`
(
IN inspectionId BIGINT)
SQL SECURITY DEFINER
BEGIN
    CALL SpGetInspectionAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'INSPECTION_LABEL_ATTRIBUTES\' ) AND IA.INSPECTION_ATTRIBUTE_INSPECTION_ID =',  inspectionId ) );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetInspectionStatisticAttributes`
(
IN inspectionId BIGINT)
SQL SECURITY DEFINER
BEGIN
    CALL SpGetInspectionAttributes( CONCAT( 'WHERE A.ATTRIBUTE_PARENT_ID = FcGetAttributeId( \'INSPECTION_STATISTIC_ATTRIBUTES\' ) AND IA.INSPECTION_ATTRIBUTE_INSPECTION_ID =',  inspectionId ) );
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSgGeometricZone`
(
IN geometricZoneId BIGINT,
IN steamGeneratorId BIGINT
)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the SG_GEOMETRIC_ZONES table or update an existing record'
BEGIN
    
	IF NOT FcSgGeometricZoneExist( geometricZoneId, steamGeneratorId ) THEN

        INSERT INTO SG_GEOMETRIC_ZONES 
        (
            SG_GEOMETRIC_ZONE_ID,
            SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID,
            SG_GEOMETRIC_ZONE_CREATION_DATE
        )
        VALUES
        (
            geometricZoneId,
            steamGeneratorId,
            NOW()
        );

    ELSE

        UPDATE 
            SG_GEOMETRIC_ZONES 
        SET
            SG_GEOMETRIC_ZONE_ID = geometricZoneId,
            SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId,
            SG_GEOMETRIC_ZONE_MODIFICATION_DATE = NOW()
        WHERE
            SG_GEOMETRIC_ZONE_ID = geometricZoneId
        AND
            SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId;

    END IF;

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpClearSgGeometricZonesTable`
()
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Clears the SG_GEOMETRIC_ZONE  table'
BEGIN

    CALL SpDeleteSgGeometricZones( '' );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSgGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Allows to delete records in the SG_GEOMETRIC_ZONES table matching the whereClause'
BEGIN

    CALL SpDeleteRecords( 'SG_GEOMETRIC_ZONES', whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSgGeometricZones`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
COMMENT 'Retrieves all records present in the SG_GEOMETRIC_ZONES table matching the where clause.'
BEGIN

    CALL SpGetRecords( 'SELECT
                            SGGZ.SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID AS STEAM_GENERATOR_ID,
                            SGGZ.SG_GEOMETRIC_ZONE_ID AS GEOMETRIC_ZONE_ID,
                            SG.STEAM_GENERATOR_LOW_ROW AS LOW_ROW,
                            GZ.GEOMETRIC_ZONE_TYPE_ID AS TYPE_ID,
                            GZ.GEOMETRIC_ZONE_NAME AS NAME,
                            GZ.GEOMETRIC_ZONE_LENGTH AS LENGTH,                          
                            SGGZ.SG_GEOMETRIC_ZONE_CREATION_DATE AS CREATION_DATE,
                            SGGZ.SG_GEOMETRIC_ZONE_MODIFICATION_DATE AS MODIFICATION_DATE
                        FROM
                            SG_GEOMETRIC_ZONES SGGZ
                                INNER JOIN GEOMETRIC_ZONES GZ ON GZ.GEOMETRIC_ZONE_ID = SGGZ.SG_GEOMETRIC_ZONE_ID
                                INNER JOIN STEAM_GENERATORS SG ON SG.STEAM_GENERATOR_ID = SGGZ.SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID'
                    ,whereClause );                       

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSgGeometricZone`
(
IN geometricZoneId BIGINT,
IN steamGeneratorId BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the SG_GEOMETRIC_ZONES table'
BEGIN

    DELETE FROM 
        SG_GEOMETRIC_ZONES
    WHERE
        SG_GEOMETRIC_ZONE_ID = geometricZoneId
    AND
        SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetGeometricZoneTypes`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

	 CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            IF(A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'GEOMETRIC_ZONE_TYPES\' ),NULL, A.ATTRIBUTE_PARENT_ID ) AS PARENT_ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
                         WHERE A.ATTRIBUTE_FULL_NAME LIKE \'GEOMETRIC_ZONE_TYPES.%\''
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteGeometricZoneType`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateGeometricZoneType`
(
INOUT id BIGINT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (GEOMETRIC_ZONE_TYPES) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'GEOMETRIC_ZONE_TYPES' ),
                               NULL,
                               NULL,
                               NULL,
                               name,
                               label,
                               labelTranslationKey,
                               NULL,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpAddUpdateSteamGeneratorTypeFormula`
(
INOUT id BIGINT,
IN name VARCHAR(30),
IN label VARCHAR(50),
IN labelTranslationKey VARCHAR(50),
OUT date DATETIME)
MODIFIES SQL DATA
SQL SECURITY DEFINER
COMMENT 'Allows to add a new record in the ATTRIBUTES table or update an existing record (STEAM_GENERATOR_TYPE_FORMULAS) virtual table'
BEGIN

    CALL SpAddUpdateAttribute( id,
                               FcGetAttributeId( 'STEAM_GENERATOR_TYPE_FORMULAS' ),
                               NULL,
                               NULL,
                               NULL,
                               name,
                               label,
                               labelTranslationKey,
                               NULL,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               date );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpGetSteamGeneratorTypeFormulas`
(
IN whereClause VARCHAR(500))
SQL SECURITY DEFINER
BEGIN

     CALL SpGetRecords( 'SELECT
                            A.ATTRIBUTE_ID AS ID,
                            IF(A.ATTRIBUTE_PARENT_ID=FcGetAttributeId( \'STEAM_GENERATOR_TYPE_FORMULAS\' ),NULL, A.ATTRIBUTE_PARENT_ID ) AS PARENT_ID,
                            A.ATTRIBUTE_NAME AS NAME,
                            A.ATTRIBUTE_LABEL AS LABEL,
                            A.ATTRIBUTE_LABEL_TRANSLATION_KEY AS LABEL_TRANSLATION_KEY,
                            A.ATTRIBUTE_CREATION_DATE AS CREATION_DATE,
                            A.ATTRIBUTE_MODIFICATION_DATE AS MODIFICATION_DATE
                         FROM
                            ATTRIBUTES A 
                         WHERE A.ATTRIBUTE_FULL_NAME LIKE \'STEAM_GENERATOR_TYPE_FORMULAS.%\''
                    ,whereClause );

END
;

$$

CREATE DEFINER = `ariaUser`@`%` PROCEDURE `SpDeleteSteamGeneratorTypeFormula`
(
IN id BIGINT)
SQL SECURITY DEFINER
COMMENT 'Allows to delete an existing record in the ATTRIBUTES table'
BEGIN

    CALL SpDeleteAttribute( id );
    
END
;

$$

-- Create functions section -------------------------------------------------

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcGetAttributeId`
(
fullName VARCHAR(100))
RETURNS int(11)
SQL SECURITY DEFINER
BEGIN
    
    DECLARE attributeId BIGINT DEFAULT NULL;
    
    SELECT
        ATTRIBUTE_ID
    INTO
        attributeId
    FROM
        ATTRIBUTES
    WHERE
        ATTRIBUTE_FULL_NAME = fullName;
	
	RETURN attributeId;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcGetAttributeName`
(
attributeId BIGINT)
RETURNS varchar(30) CHARSET utf8
SQL SECURITY DEFINER
BEGIN
    
    DECLARE name VARCHAR(30) DEFAULT NULL;
    
    SELECT
        ATTRIBUTE_NAME
    INTO
        name
    FROM
        ATTRIBUTES
    WHERE
        ATTRIBUTE_ID = attributeId;
	
	RETURN name;
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEquipmentExist`
(
equipmentId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EQUIPMENTS
        WHERE
            EQUIPMENT_ID = equipmentId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcApplicationFunctionalityExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            APPLICATION_FUNCTIONALITIES
        WHERE
            APPLICATION_FUNCTIONALITY_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcAttributeExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ATTRIBUTES
        WHERE
            ATTRIBUTE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcComponentExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            COMPONENTS
        WHERE
            COMPONENT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcControlAttributeExist`
(
controlId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            CONTROLS_ATTRIBUTES
        WHERE
            CONTROL_ATTRIBUTE_CONTROL_ID = controlId
        AND
            CONTROL_ATTRIBUTE_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcControlExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            CONTROLS
        WHERE
            CONTROL_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcControlZoneExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            CONTROL_ZONES
        WHERE
            CONTROL_ZONE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcConversionExist`
(
unitSourceId BIGINT,
unitDestId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            CONVERSIONS
        WHERE
            CONVERSION_UNIT_SOURCE_ID = unitSourceId
        AND
            CONVERSION_UNIT_DEST_ID = unitDestId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcConverterExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            CONVERTERS
        WHERE
            CONVERTER_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEquipmentAttributeExist`
(
equipmentId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EQUIPMENTS_ATTRIBUTES
        WHERE
            EQUIPMENT_ATTRIBUTE_EQUIPMENT_ID = equipmentId
        AND
            EQUIPMENT_ATTRIBUTE_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEquipmentTrackingExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EQUIPMENT_TRACKINGS
        WHERE
            EQUIPMENT_TRACKING_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEquipmentTypeAttributeExist`
(
equipmentTypeId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EQUIPMENT_TYPES_ATTRIBUTES
        WHERE
            ETA_EQUIPMENT_TYPE_ID = equipmentTypeId
        AND
            ETA_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcFileExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            FILES
        WHERE
            FILE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcAcquisitionFileExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ACQUISITION_FILES
        WHERE
            ACQUISITION_FILE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcAcquisitionLineExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ACQUISITION_LINES
        WHERE
            ACQUISITION_LINE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcAcquisitionExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ACQUISITIONS
        WHERE
            ACQUISITION_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcAcquisitionGroupExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ACQUISITION_GROUPS
        WHERE
            ACQUISITION_GROUP_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEquipmentTrackingAttributeExist`
(
equipmentTrackingId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EQUIPMENT_TRACKINGS_ATTRIBUTES
        WHERE
            ETA_EQUIPMENT_TRACKING_ID = equipmentTrackingId
        AND
            ETA_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcRoleExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ROLES
        WHERE
            ROLE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcUserExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            USERS
        WHERE
            USER_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcExamExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EXAMS
        WHERE
            EXAM_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcLanguageExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            LANGUAGES
        WHERE
            LANGUAGE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTextExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TEXTS
        WHERE
            TEXT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEquipmentTypeExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EQUIPMENT_TYPES
        WHERE
            EQUIPMENT_TYPE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcGeometricZoneExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            GEOMETRIC_ZONES
        WHERE
            GEOMETRIC_ZONE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcInspectionExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            INSPECTIONS
        WHERE
            INSPECTION_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcIOExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            IOS
        WHERE
            IO_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcLayoutExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            LAYOUTS
        WHERE
            LAYOUT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcManwayExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            MANWAYS
        WHERE
            MANWAY_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcNuclearPlantUnitExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            NUCLEAR_PLANT_UNITS
        WHERE
            NUCLEAR_PLANT_UNIT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcOrganExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ORGANS
        WHERE
            ORGAN_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcQcResultExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            QUALITY_CHECK_RESULTS
        WHERE
            QUALITY_CHECK_RESULT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcQcResultValueExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            QUALITY_CHECK_RESULT_VALUES
        WHERE
            QUALITY_CHECK_RESULT_VALUE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcSiteExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            SITES
        WHERE
            SITE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcSteamGeneratorTypeExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            STEAM_GENERATOR_TYPES
        WHERE
            STEAM_GENERATOR_TYPE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTechniqueExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TECHNIQUES
        WHERE
            TECHNIQUE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTubeExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TUBES
        WHERE
            TUBE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTubeGeometricZoneExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TUBES_GEOMETRIC_ZONES
        WHERE
            TUBES_GEOMETRIC_ZONE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcZoneExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ZONES
        WHERE
            ZONE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcOrganAttributeExist`
(
organId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ORGANS_ATTRIBUTES
        WHERE
            ORGAN_ATTRIBUTE_ORGAN_ID = organId
		AND
			ORGAN_ATTRIBUTE_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcSgTubeExist`
(
tubeId BIGINT,
steamGeneratorId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            SG_TUBES
        WHERE
            SG_TUBE_TUBE_ID = tubeId
		AND
			SG_TUBE_STEAM_GENERATOR_ID = steamGeneratorId

    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcSgTubeGeometricZoneExist`
(
tubeGeometricZoneId BIGINT,
steamGeneratorId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            SG_TUBES_GEOMETRIC_ZONES
        WHERE
            SG_TUBES_GEOMETRIC_ZONE_ID = tubeGeometricZoneId
		AND
			SG_TUBES_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId

    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTubeZoneExist`
(
TubeId BIGINT,
zoneId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TUBES_ZONES
        WHERE
            TUBE_ZONE_ZONE_ID = zoneId
		AND
			TUBE_ZONE_TUBE_ID = TubeId

    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcControlSelectedEquipmentExist`
(
controlId BIGINT,
equipmentId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            SELECTED_EQUIPMENTS
        WHERE
            SELECTED_EQUIPMENT_CONTROL_ID = controlId
        AND
            SELECTED_EQUIPMENT_EQUIPMENT_ID = equipmentId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcEventExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EVENTS
        WHERE
            EVENT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcInspectionSelectedControlExist`
(
inspectionId BIGINT,
controlId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            SELECTED_CONTROLS
        WHERE
            SELECTED_CONTROL_CONTROL_ID = controlId
        AND
            SELECTED_CONTROL_INSPECTION_ID = inspectionId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcMessageExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            MESSAGES
        WHERE
            MESSAGE_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcPokActionExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            POK_ACTIONS
        WHERE
            POK_ACTION_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcBackupHistoryExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            BACKUP_HISTORIES
        WHERE
            BACKUP_HISTORY_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcQueueFileTransferExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            QUEUE_FILES_TRANSFER
        WHERE
            QUEUE_FILE_TRANSFER_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTechniqueAttributeExist`
(
techniqueId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TECHNIQUES_ATTRIBUTES
        WHERE
            TECHNIQUE_ATTRIBUTE_TECHNIQUE_ID = techniqueId
		AND
			TECHNIQUE_ATTRIBUTE_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTechniqueEquipmentTypeExist`
(
techniqueId BIGINT,
equipmentTypeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TECHNIQUES_EQUIPMENT_TYPES
        WHERE
            TET_TECHNIQUE_ID = techniqueId
		AND
			TET_EQUIPMENT_TYPE_ID = equipmentTypeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcDatabaseVersionExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            DATABASE_VERSIONS
        WHERE
            DATABASE_VERSION_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcInspectionAttributeExist`
(
inspectionId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            INSPECTIONS_ATTRIBUTES
        WHERE
            INSPECTION_ATTRIBUTE_INSPECTION_ID = inspectionId
		AND
			INSPECTION_ATTRIBUTE_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTaskExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            TASKS
        WHERE
            TASK_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcActorExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            ACTORS
        WHERE
            ACTOR_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcUnitConfigurationExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            UNIT_CONFIGURATIONS
        WHERE
            UNIT_CONFIGURATION_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcUnitExist`
(
id BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            UNITS
        WHERE
            UNIT_ID = id
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcControlTubeExist`
(
controlId BIGINT,
tubeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            CONTROLS_TUBES
        WHERE
            CONTROL_TUBE_CONTROL_ID = controlId
        AND
            CONTROL_TUBE_TUBE_ID = tubeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcExamAttributeExist`
(
examId BIGINT,
attributeId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EXAMS_ATTRIBUTES
        WHERE
            EXAM_ATTRIBUTE_EXAM_ID = examId
		AND
			EXAM_ATTRIBUTE_ATTRIBUTE_ID = attributeId
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcTubeIsToControl`
(
tubeId BIGINT,
controlId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            EXAMS E 
                LEFT OUTER JOIN ACQUISITIONS A ON A.ACQUSITION_EXAM_ID = E.EXAM_ID
        WHERE
            E.EXAM_CONTROL_ID = controlId
        AND 
            E.EXAM_TUBE_ID = tubeId
        AND
            A.ACQUISITION_QUALITY_CHECK_STATE = 0 
    ),FALSE);
    
END
;

$$

CREATE DEFINER = `ariaUser`@`%` FUNCTION `FcSgGeometricZoneExist`
(
geometricZoneId BIGINT,
steamGeneratorId BIGINT)
RETURNS tinyint(1)
SQL SECURITY DEFINER
BEGIN
    
	RETURN IFNULL(
    (
        SELECT 
            TRUE
        FROM
            SG_GEOMETRIC_ZONES
        WHERE
            SG_GEOMETRIC_ZONE_ID = geometricZoneId
        AND
            SG_GEOMETRIC_ZONE_STEAM_GENERATOR_ID = steamGeneratorId

    ),FALSE);
    
END
;

$$



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
