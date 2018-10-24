from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'HYDJET_PbPb_1000'
config.General.workArea = 'output_files'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../test/run_PbPb_HYDJET_0_5_Prabhat_cfg.py'
config.JobType.inputFiles = ['EffCorrectionsPixelPbPb_nominal_v2.root']
config.JobType.maxJobRuntimeMin = 2750 

config.Data.inputDataset = '/Hydjet_Quenched_MinBias_5020GeV_750/HINPbPbWinter16DR-NoPU_75X_mcRun2_HeavyIon_v13_75X_mcRun2_HeavyIon_v13-v1/AODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1000 
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/HI/Cert_262548-263757_PromptReco_HICollisions15_JSON_v2.txt'
#config.Data.runRange = '193093-193999' # '193093-194075'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_test_output'

config.Site.storageSite = 'T2_US_Nebraska'
