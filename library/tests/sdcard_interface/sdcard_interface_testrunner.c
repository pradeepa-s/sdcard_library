#include "unity_fixture.h"

TEST_GROUP_RUNNER(sdcard_interface)
{
	RUN_TEST_CASE(sdcard_interface,	InitFunctionClearsBufferPointerAndSize);	
	RUN_TEST_CASE(sdcard_interface,	InitFunctionSetsTheEventLogFileToDefault);
	RUN_TEST_CASE(sdcard_interface,	InitFunctionCreatesTheDefaultLogIfNotAlreadyThere);
	
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferErrorIfInitIsNotCalled );
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferReturnsErrorIfBufferPointerIsNull);
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferReturnsErrorIfBufferSizeIsZero);	
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferReturnsSuccessIfParametersAreCorrectAndInitCalled);
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferSetsInternalBufferPointer);
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferSetsInternalBufferSize)
	RUN_TEST_CASE(sdcard_interface, SetAudioFileBufferReturnsErrorIfBufferSizeIsLessthanZero);
	
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileErrorIfInitIsNotCalled);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileErrorIfFileNameIsNULL);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileErrorIfFileNotAvailable);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileWarnIfFileBiggerThanBuffer);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileCopyOnlyBufferSizeIfFileIsBiggerThanBuffer);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileWarnIfFileSmallerThanBuffer);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileCopyFullFileContentIfFileIfSmallerThanBuffer);
	RUN_TEST_CASE(sdcard_interface, PlayAudioFileErrorIfBufferIsNULL);
	RUN_TEST_CASE(sdcard_interface,	PlayAudioFileErrorIfBufferSizeIsZero);
	RUN_TEST_CASE(sdcard_interface,	PlayAudioFileCopiesDataCorrectly);	
	
	RUN_TEST_CASE(sdcard_interface,	SetLogFileErrorIfFileNameIsNULL);
	RUN_TEST_CASE(sdcard_interface,	SetLogFileErrorIfInitIsNotCalled);
	RUN_TEST_CASE(sdcard_interface,	SetLogFileReturnsSuccessIfFileIsAvailable);
	RUN_TEST_CASE(sdcard_interface,	SetLogFileReturnsSuccessIfFileIsNotAvailableAndCreateFile);
	RUN_TEST_CASE(sdcard_interface,	SetLogFileSetsTheLogFile);
	RUN_TEST_CASE(sdcard_interface,	SetLogFileDoesNotChangeLogFileIfErrored);
	RUN_TEST_CASE(sdcard_interface,	SetLogFileErrorIfFilenameTooLarge);
	
	RUN_TEST_CASE(sdcard_interface,	DeleteLogFileGeneratesErrorIfFilenameIsNULL);
	RUN_TEST_CASE(sdcard_interface,	DeleteLogFileGeneratesErrorIfFileNotAvailable);
	RUN_TEST_CASE(sdcard_interface,	DeleteLogFileDeletesTheFileIfFileIsAvailable);
	RUN_TEST_CASE(sdcard_interface,	DeleteLogFileSetsTheDefaultLogFileIfItDeletesTheCurrentLogFile);
	
	RUN_TEST_CASE(sdcard_interface,	GetCurrentLogFileGeneratesErrorIfFilenameIsNULL);
	RUN_TEST_CASE(sdcard_interface,	GetCurrentLogFileGeneratesErrorIfBufferSizeIsLessThanOrEqualsToZero);
	RUN_TEST_CASE(sdcard_interface,	GetCurrentLogFileGeneratesErrorIfBufferSizeIsNotEnough);
	RUN_TEST_CASE(sdcard_interface,	GetCurrentLogFileGeneratesErrorIfModuleNotInitialized);
	RUN_TEST_CASE(sdcard_interface,	GetCurrentLogCopiesTheFilenameToTheProvidedBuffer);
	
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfFilenameIsNULL);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfBufferIsNULL);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfBufferSizeIsNULL);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfBufferSizeLessThanOrEqualsToZero);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfProvidedOffsetIsLessThanZero);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfProvidedOffsetIsMoreThanFilesize);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesErrorIfFilesizeParameterNULL);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesCopiesFilesizeWhenBufferSizeIsInvalid);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesCopiesFilesizeWhenBufferOffsetIsMore);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesCopiesFilesizeWhenBufferOffsetIsInvalid);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesCopiesFilesizeWhenSuccess);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesCopiesCopiedAmountToBufSize);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileGeneratesStartsDataCopyingAtTheOffset);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileCopiesDataUntilBufferIsFullWhenFileIsBiggerAndGeneratesWarning);
	RUN_TEST_CASE(sdcard_interface,	ReadFirmwareFileCopiesDataUntilBufferIsFullWhenFileIsSmallerAndGeneratesWarning);
	
	RUN_TEST_CASE(sdcard_interface,	LogEventReturnsErrorIfInitNotCalled);
	RUN_TEST_CASE(sdcard_interface,	LogEventReturnsErrorIfEventsStructureIsNULL);
	RUN_TEST_CASE(sdcard_interface,	LogEventOpenFileIfAvailableAndAppendsEvent);
	RUN_TEST_CASE(sdcard_interface,	LogEventAppendsEventsInNewLines);
	
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfEventsStructureIsNULL);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfFileNotAvailable);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfNoOfEventsParamIsNULL);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfEventBufferIsNULL);	
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfReadTypeIsInvalid);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsWarningIfRequiredNoOfEventsByParameterAreNotAvailable);	
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfNoOfEventsIsZeroUnlessReadTypeLast100);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfNoOfEventsIsZeroReturnsRequiredAmount);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsWarningIf100EventsNotAvailableWhenLast100Used);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsReadNumberOfEventsWhenLessEventsAreThereForLast100);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsNoOfEventsAs100IfEventsAreThereForLast100);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesAvailableEventsIf100EventsAreNotThereForLast100);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopies100EventsIf100EventsAreThereForLast100);
	RUN_TEST_CASE(sdcard_interface,	ReadEventReturnsErrorIfFULL_READusedButBufferNotEnough);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesRequiredBufSizeIfFULL_READusedButBufferNotEnough);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesAllTheEventsAvailable);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesAvailableEventsIfNisMoreForN_FROM_BEGINING);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesNEventsIfNEventsAvailableForN_FROM_BEGINING);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesAvailableEventsIfNisMoreForN_FROM_LAST);
	RUN_TEST_CASE(sdcard_interface,	ReadEventCopiesNEventsIfNEventsAvailableForN_FROM_LAST);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_detects_offset_out_of_bound_for_N_FROM_LAST);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_detects_negetive_offset_for_N_FROM_LAST);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_detects_offset_out_of_bound_for_N_FROM_BEGINING);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_detects_negetive_offset_for_N_FROM_BEGINING);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_copies_data_with_offset_correctly_for_N_FROM_LAST);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_with_offset_for_N_FROM_LAST_copies_available_data_when_required_number_of_events_not_available);
	RUN_TEST_CASE(sdcard_interface,	ReadEvent_with_offset_for_N_FROM_BEGINING_copies_available_data_when_required_number_of_events_not_available);	
	
	RUN_TEST_CASE(sdcard_interface,	CreateFirmwareFile_errorIfFilenameIsNull);
	RUN_TEST_CASE(sdcard_interface,	CreateFirmwareFile_errorIfFileIsAlreadyThere);
	//RUN_TEST_CASE(sdcard_interface,	CreateFirmwareFile_errorIfFileCreationFailed);
	RUN_TEST_CASE(sdcard_interface,	CreateFirmwareFile_CreatesAnEmptyFile);
	
	RUN_TEST_CASE(sdcard_interface,	DeleteFirmwareFile_errorIfFilenameIsNull);
	RUN_TEST_CASE(sdcard_interface,	DeleteFirmwareFile_successIfFileNotthere);
	//RUN_TEST_CASE(sdcard_interface,	DeleteFirmwareFile_errorIfFileDeletionFailed);
	RUN_TEST_CASE(sdcard_interface,	DeleteFirmwareFile_DeletesTheFile);
	
	RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_errorIfFilenameIsNull);
	RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_errorIfFileNotAvailable);
	//RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_errorIfFileNotAccessible);
	RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_errorIfDataBufferIsNull);
	RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_errorIfDataLengthIsZero);
	RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_errorIfDataLengthIsNegative);
	RUN_TEST_CASE(sdcard_interface,	AppendFirmwareData_AppendsDataToTheEndOfFile);
	
	
	RUN_TEST_CASE(sdcard_interface,	GetCurrentLogFileCopiesTheLogFileNameToTheBuffer);
	
}
