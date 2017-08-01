import FWCore.ParameterSet.Config as cms

process = cms.Process("LambdaCData")

process.load("FWCore.MessageService.MessageLogger_cfi")

#initialize MessageLogger and output report
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.categories.append('LambdaCData')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

	#Data test
	'file:/storage1/osg/stage_out/store/user/davidlw/PAHighMultiplicity1/RecoSkim2016_pPb_V0Cascade_FullSkim_v1/170301_205416/0000/pPb_HM_100.root'
    )
)

process.lcreco = cms.EDAnalyzer('LambdaCData', KshortCollection=cms.untracked.InputTag('generalV0CandidatesNew', 'Kshort'))

#specify output file
process.TFileService = cms.Service("TFileService", fileName = cms.string('data_LambdaC_K.root'))

process.p = cms.Path(process.lcreco)