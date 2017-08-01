from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

# GLOBAL STUFF
requestNameTag = '_histos_y'

config.section_("General")
config.General.workArea = 'crab_K' + requestNameTag
config.General.transferOutputs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/data_K_cfg.py'

config.section_("Data")
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.publishDBS = 'phys03'
config.Data.useParent = True
config.Data.ignoreLocality = True
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())

config.section_("Site")
config.Site.storageSite = 'T3_US_Rice'
config.Site.whitelist = ['T2_US_Vanderbilt']

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

    def submit_task(config, setname, index, requestNameTag):
    # PARAMETERS TO VARY WITH EACH TASK

        config.General.requestName = 'LambdaC_K_data_' + setname + index + requestNameTag
        config.Data.inputDataset = '/PAMinimumBias' + index + '/davidlw-RecoSkim2016_' + setname + '_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER'
        config.Data.outputDatasetTag = 'CRAB3_' + config.General.requestName
        submit(config)
    # END OF PARAMETERS TO VARY WITH EACH TASK

    ####pPb####
    for index in range(1, 9):
        print
        print('Submitting task: pPb' + str(index))
        submit_task(config, 'pPb', str(index), requestNameTag)
        print

    ####Pbp####
    for index in []:
        print
        print('Submitting task: Pbp' + str(index))
        submit_task(config, 'Pbp', str(index), requestNameTag)
        print

    print('Tasks have successfully been submitted!')