from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

# GLOBAL STUFF
requestNameTag = '_gsb'

config.section_("General")
config.General.workArea = 'crab_projects_MC'
config.General.transferOutputs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/MC_L_cfg.py'

config.section_("Data")
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.publishDBS = 'phys03'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())

config.section_("Site")
config.Site.storageSite = 'T3_US_Rice'

# END OF GLOBAL STUFF

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    # PARAMETERS TO VARY WITH EACH TASK

    i = 0;
    print
    print('Submitting task: pt_hat = ' + str(i))
    config.General.requestName = 'LambdaC_L_MC_pthat' + str(i) + requestNameTag
    config.Data.inputDataset = '/Pythia8_8160GeV_LambdaC_LambdaPion/zhchen-Pythia8_8160GeV_LambdaC_LambdaPion_AODSIM_v5-batch1_NoPAngle_NoDL-517fbb26312f777f94848083dc6dc607/USER'
    config.Data.outputDatasetTag = 'CRAB3_' + config.General.requestName
    submit(config)
    print

    for i in [10, 20, 40, 80, 120]:
        print
        print('Submitting task: pt_hat = ' + str(i))
        config.General.requestName = 'LambdaC_L_MC_pthat' + str(i) + requestNameTag
        config.Data.inputDataset = '/Pythia8_8160GeV_LambdaC_LambdaPion_GENSIM/zhchen-Pythia8_8160GeV_LambdaC_LambdaPion_pthat' + \
            str(i) + '_AODSIM_v6-batch1-517fbb26312f777f94848083dc6dc607/USER'
        config.Data.outputDatasetTag = 'CRAB3_' + config.General.requestName
        submit(config)
        print

    # END OF PARAMETERS TO VARY WITH EACH TASK


    print('Tasks have successfully been submitted!')