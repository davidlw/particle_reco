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

	#MC LambdaPi test
	'root://cms-xrd-global.cern.ch//store/user/zhchen/Pythia8_8160GeV_LambdaC_LambdaPion/Pythia8_8160GeV_LambdaC_LambdaPion_AODSIM_v5-batch1_NoPAngle_NoDL/170530_210458/0000/Pythia8_8160GeV_LambdaC_LambdaPion_step2_1.root'
	
    )
)

process.lcreco = cms.EDAnalyzer('LambdaCData', LambdaCollection=cms.untracked.InputTag('generalV0Candidates', 'Lambda'))

#specify output file
process.TFileService = cms.Service("TFileService", fileName = cms.string('MC_LambdaC_L.root'))

process.p = cms.Path(process.lcreco)
