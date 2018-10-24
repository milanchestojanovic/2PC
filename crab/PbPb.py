from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'PbPb_cent_15_35a'
config.General.workArea = 'output_files'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../test/run_PbPb_15_35_PrabhatComp_cfg.py'
config.JobType.inputFiles = ['EffCorrectionsPixelPbPb_nominal.root']
config.JobType.maxJobRuntimeMin = 2750 

config.Data.inputDataset = '/HIMinimumBias2/HIRun2015-25Aug2016-v1/AOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 10 
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/HI/Cert_262548-263757_PromptReco_HICollisions15_JSON_v2.txt'
#config.Data.runRange = '193093-193999' # '193093-194075'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_test_output'

config.Site.storageSite = 'T2_US_Nebraska'
