from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

# GLOBAL STUFF
requestNameTag = '_vtx'

config.section_("General")
config.General.workArea = 'crab_projects_MC'
config.General.transferOutputs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'

config.section_("Data")
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.useParent = True
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

    # JOB-SPECIFIC PARAMETERS

    print
    print('Submitting task: KsProton')
    config.JobType.psetName = '../python/MC_K_vtx_cfg.py'
    config.General.requestName = 'LambdaC_K_MC' + requestNameTag;
    config.Data.inputDataset = '/Pythia8_8160GeV_LambdaC_KsProton/davidlw-LambdaCMC_KsP_v1-5aef785c78d45cd0f1d103f789c0fc2d/USER'
    config.Data.outputDatasetTag = 'CRAB3_' + config.General.requestName
    submit(config)
    print

    # END OF JOB-SPECIFIC PARAMETERS

    print('Tasks have successfully been submitted!')
