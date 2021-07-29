import FWCore.ParameterSet.Config as cms

process = cms.Process('TRACKANA')
#process.load("CmsHi.JetAnalysis.HiForest_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
process.load("CondCore.CondDB.CondDB_cfi")
process.load('2pc.BigFlow.BigFlowAnalyzer_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('/eos/user/m/mstojano/Run3/miniAOD/corr_miniAOD_MB.root')
)


# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring(
  'file:/eos/cms/store/group/phys_heavyions/caber/MiniAODValidation/MiniAODfiles/step2_HIMiniAOD.root'
),

    #eventsToProcess = cms.untracked.VEventRange('1:6652:352538')
     #eventsToProcess = cms.untracked.VEventRange('327560:391:189830534')
     #lumisToProcess = cms.untracked.VLuminosityBlockRange('327516:63')
)
### centrality ###

#process.HeavyIonGlobalParameters = cms.PSet(
#    centralitySrc = cms.InputTag("hiCentrality"),
#    centralityBinSrc = cms.InputTag("centralityBin","HFtowers"),
#)



### Track cuts ###
# input collections
process.BigFlow.vertexSrc = cms.InputTag("offlineSlimmedPrimaryVerticesRecovery")
process.BigFlow.centralitySrc = cms.InputTag("centralityBin","HFtowers")
#process.BigFlow.trackSrc = cms.InputTag("hiConformalPixelTracks")
process.BigFlow.trackSrc = cms.InputTag("generalTracks")
process.BigFlow.qualityString = cms.string("highPurity")
process.BigFlow.pfCandSrc = cms.untracked.InputTag("particleFlowTmp")
process.BigFlow.jetSrc = cms.InputTag("akPu4CaloJets")
# options
process.BigFlow.doEffCorr = False 
process.BigFlow.useCentrality = True 
process.BigFlow.applyTrackCuts = True
process.BigFlow.fillNTuples = False
process.BigFlow.applyVertexZCut = True
process.BigFlow.doVtxReweighting = False
process.BigFlow.doCaloMatched = False
process.BigFlow.doPixels = False

# cut values
process.BigFlow.dxyErrMax = 3.0
process.BigFlow.dzErrMax = 3.0
process.BigFlow.ptErrMax = 0.1
process.BigFlow.nhitsMin = 11 
process.BigFlow.chi2nMax = 0.18
process.BigFlow.chi2nMax_pixel = 999.9 
process.BigFlow.dzErrMax_pixel = 20
process.BigFlow.dxyErrMax_pixel = 70

process.BigFlow.reso = 0.2
#process.BigFlow.effTable = "/afs/cern.ch/work/m/mstojano/private/CENT_TEST/XeXe/CMSSW_9_2_12_patch1/src/2pc/BigFlow/test/XeXe_eff_table_92x.root"
process.BigFlow.effTable = "EffCorrectionsGenTrkPbPb_Preliminary.root"
process.BigFlow.effTablePhi = "EffCorrectionsPixel_Phi.root"
#process.HITrackCorrections.crossSection = 1.0 #1.0 is no reweigh
#algo 
process.BigFlow.algoParameters = cms.vint32(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#events of interest
process.BigFlow.centmax = 100
process.BigFlow.centmin = 0 
#Analysis parameters
process.BigFlow.pt_trigmax = cms.vdouble(3.0)
process.BigFlow.pt_trigmin = cms.vdouble(3.0)
process.BigFlow.pt_assmax = cms.vdouble(.6,.8,1.,1.25,1.5,2.0,2.5,3.0) #,3.5,4.0,5.,6.,7.,8.,10.)
process.BigFlow.pt_assmin = cms.vdouble(.5,.6,.8,1.0,1.25,1.5,2.0,2.5) #,3.0,3.5,4.,5.,6.,7.,8.0)
process.BigFlow.pt_tbins = 1 
process.BigFlow.pt_nbins = 8 
# vertex reweight parameters
process.BigFlow.vtxWeightParameters = cms.vdouble(0.0306789, 0.427748, 5.16555, 0.0228019, -0.02049, 7.01258 )

## event selection

# trigger selection

###

#GlobalTak

#Centrality

#process.load('Configuration.StandardSequences.Services_cff')
#process.load('Configuration.Geometry.GeometryDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('FWCore.MessageService.MessageLogger_cfi')


# Set the global tag

#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data_promptlike_hi', '')
"""
# Add PbPb centrality
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("HeavyIonRcd"),
        tag = cms.string("CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run2v1031x02_offline"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        label = cms.untracked.string("HFtowers")
        ),
    ])
"""
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
#process.load('HeavyIonsAnalysis.Configuration.hfCoincFilter_cff')
#process.load('HeavyIonsAnalysis.EventAnalysis.hffilter_cfi')

# Define the event selection sequence
process.eventFilter = cms.Sequence(
    process.phfCoincFilter2Th4 *
    process.primaryVertexFilter *
    process.clusterCompatibilityFilter
)

#from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import overrideJEC_PbPb5020
#process = overrideJEC_PbPb5020(process)

#process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
#process.centralityBin.Centrality = cms.InputTag("hiCentrality")
#process.centralityBin.centralityVariable = cms.string("HFtowers")
###
process.p = cms.Path(
#		      process.hiCentrality *
#                      process.HiForest *
		      #process.centralityBin *
		      process.eventFilter *
                      process.BigFlow
)
