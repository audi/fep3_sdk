//
// Copyright @ 2019 Audi AG. All rights reserved.
// 
//     This Source Code Form is subject to the terms of the Mozilla
//     Public License, v. 2.0. If a copy of the MPL was not distributed
//     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
// 
// If it is not possible or desirable to put the notice in a particular file, then
// You may include the notice in a location (such as a LICENSE file in a
// relevant directory) where a recipient would be likely to look for such a notice.
// 
// You may add additional accurate notices of copyright ownership.

@Library(["cortex_jenkins_pipeline_lib", "cortex_jenkins_lib_settings"]) _

conan.settings = cortex_jenkins_lib_settings(gitflow: true)
conan.settings.extraModules = ['wheel', 'sphinx', 'sphinx_rtd_theme', 'breathe==4.16.0'] // required for ARM build and documentation

def conanfileDir = "."
def user = 'aev25'

def mailTo = 'pierre.voigtlaender@audi.de' 

//skipIndexingBuilds()

pipeline {
    agent none
    
	triggers {
		cron(env.BRANCH_NAME == 'develop' ? 'H 0 * * 1-5' : '')
	}

    options {
        skipDefaultCheckout()
    }

    stages {
        stage('Platform Lanes') {
            parallel {
                stage('U1604_x64_gcc54') {
                    agent { label "U1604_x64_gcc54 && AEV25" }
                    stages {
                        stage('Build Debug and Release') {
                            steps {
                                conanCreateUploadReleaseDebug(profile: 'U1604_x64_gcc54', conanFile: conanfileDir, user: user)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                }
                /*
                stage('U1604_armv8_gcc54') {
                    agent { label "U1604_armv8_gcc54" }
                    stages {
                        stage('Build Debug and Release') {
                            steps {
                                conanCreateUploadReleaseDebug(profile: 'U1604_armv8_gcc54', conanFile: conanfileDir, user: user)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                }*/
                stage('WIN10_x64_vc140_VS2017') {
                    agent { label "WIN10_x64_vc140_VS2017 && AEV25" }
                    stages {
                        stage('Build Debug and Release') {
                            steps {
                                conanCreateUploadReleaseDebug(profile: 'WIN10_x64_vc140_VS2017', conanFile: conanfileDir, user: user)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                }
            }
        }
        stage('Deploy') {
            agent { label "Linux_x64" }
            when {
                anyOf { tag 'v*'; tag 'v*-beta' }
            }
            steps {
                conanDeploy(conanFile: conanfileDir, user: user)
            }
        }
    }
    post {
        changed {
            emailext to: mailTo, subject: "Build ${env.JOB_NAME} ${currentBuild.result}", body: "Build URL: ${env.BUILD_URL}"
        }
    }
}
