import FWCore.ParameterSet.Config as cms

process = cms.Process('TRACKANA')
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
process.load('2pc.BigFlow.BigFlowAnalyzer_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('corr.root')
)


# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring(
#'file:/afs/cern.ch/work/q/qwang/public/PbPb2015_tracking/pixeltracking_1.root'
'/store/himc/HINPbPbWinter16DR/Hydjet_Quenched_MinBias_5020GeV_750/AODSIM/NoPU_75X_mcRun2_HeavyIon_v13_75X_mcRun2_HeavyIon_v13-v1/80000/001E4607-5BBA-E611-9A99-0CC47A7E6A2C.root'
),

#    eventsToProcess = cms.untracked.VEventRange('1:6652:352538')

)
### centrality ###

#process.HeavyIonGlobalParameters = cms.PSet(
#    centralitySrc = cms.InputTag("hiCentrality"),
#    centralityBinSrc = cms.InputTag("centralityBin","HFtowers"),
#)



### Track cuts ###
# input collections
process.BigFlow.centralitySrc = cms.InputTag("centralityBin","HFtowers")
#process.BigFlow.trackSrc = cms.InputTag("generalTracks")
process.BigFlow.trackSrc1 = cms.InputTag("hiGeneralAndPixelTracks")
process.BigFlow.qualityString = cms.string("highPurity")
process.BigFlow.pfCandSrc = cms.untracked.InputTag("particleFlowTmp")
process.BigFlow.jetSrc = cms.InputTag("akPu4CaloJets")
# options
process.BigFlow.useCentrality = True 
process.BigFlow.applyTrackCuts = True
process.BigFlow.fillNTuples = False
process.BigFlow.applyVertexZCut = True
process.BigFlow.doVtxReweighting = False
process.BigFlow.doCaloMatched = False
process.BigFlow.vertexZMax = 11.0
process.BigFlow.vertexZMin = 3.0
# cut values
process.BigFlow.dxyErrMax = 2.0
process.BigFlow.dzErrMax = 2.0
process.BigFlow.ptErrMax = 0.05
process.BigFlow.nhitsMin = 11 
process.BigFlow.chi2nMax = 0.15
process.BigFlow.chi2nMax_pixel = 9 
process.BigFlow.dzErrMax_pixel = 6 
process.BigFlow.reso = 0.2
#process.BigFlow.effTable = "/afs/cern.ch/work/m/mstojano/private/CENT_TEST/XeXe/CMSSW_9_2_12_patch1/src/2pc/BigFlow/test/XeXe_eff_table_92x.root"
process.BigFlow.effTable = "EffCorrectionsPixelPbPb_nominal_v2.root"
#process.HITrackCorrections.crossSection = 1.0 #1.0 is no reweigh
#algo 
process.BigFlow.algoParameters = cms.vint32(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#events of interest
process.BigFlow.centmax = 5 
process.BigFlow.centmin = 0
#Analysis parameters
process.BigFlow.pt_trigmax = cms.vdouble(2.0)
process.BigFlow.pt_trigmin = cms.vdouble(0.3)
process.BigFlow.pt_assmax = cms.vdouble(2.0)
process.BigFlow.pt_assmin = cms.vdouble(0.3)
process.BigFlow.pt_tbins = 1
process.BigFlow.pt_nbins = 1
# vertex reweight parameters
process.BigFlow.vtxWeightParameters = cms.vdouble(0.0306789, 0.427748, 5.16555, 0.0228019, -0.02049, 7.01258 )

## event selection


#Centrality
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.centralityBin.nonDefaultGlauberModel = cms.string("HydjetDrum5")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_mcRun2_HeavyIon_v11', '')

###
process.p = cms.Path(
		      process.centralityBin *
                      process.BigFlow
)
