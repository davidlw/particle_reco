import FWCore.ParameterSet.Config as cms

process = cms.Process("LambdaCData")

process.load("FWCore.MessageService.MessageLogger_cfi")

#initialize MessageLogger and output report
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	
	#MC KsProton vertex test
	'root://cms-xrd-global.cern.ch//store/user/davidlw/Pythia8_8160GeV_LambdaC_KsProton/LambdaCMC_KsP_v1/170626_122537/0000/pPb_HM_1.root'
    )
)

process.lcreco = cms.EDAnalyzer('LambdaCData', LambdaCCollection=cms.untracked.InputTag('generalCascadeCandidatesNew', 'LambdaCToKsP'))

#specify output file
process.TFileService = cms.Service("TFileService", fileName = cms.string('MC_LambdaC_K_vtx.root'))

process.p = cms.Path(process.lcreco)