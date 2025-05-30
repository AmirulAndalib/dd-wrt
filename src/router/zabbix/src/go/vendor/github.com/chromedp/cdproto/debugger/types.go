package debugger

// Code generated by cdproto-gen. DO NOT EDIT.

import (
	"fmt"

	"github.com/chromedp/cdproto/runtime"
	"github.com/mailru/easyjson"
	"github.com/mailru/easyjson/jlexer"
	"github.com/mailru/easyjson/jwriter"
)

// BreakpointID breakpoint identifier.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-BreakpointId
type BreakpointID string

// String returns the BreakpointID as string value.
func (t BreakpointID) String() string {
	return string(t)
}

// CallFrameID call frame identifier.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-CallFrameId
type CallFrameID string

// String returns the CallFrameID as string value.
func (t CallFrameID) String() string {
	return string(t)
}

// Location location in the source code.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-Location
type Location struct {
	ScriptID     runtime.ScriptID `json:"scriptId"`               // Script identifier as reported in the Debugger.scriptParsed.
	LineNumber   int64            `json:"lineNumber"`             // Line number in the script (0-based).
	ColumnNumber int64            `json:"columnNumber,omitempty"` // Column number in the script (0-based).
}

// ScriptPosition location in the source code.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-ScriptPosition
type ScriptPosition struct {
	LineNumber   int64 `json:"lineNumber"`
	ColumnNumber int64 `json:"columnNumber"`
}

// LocationRange location range within one script.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-LocationRange
type LocationRange struct {
	ScriptID runtime.ScriptID `json:"scriptId"`
	Start    *ScriptPosition  `json:"start"`
	End      *ScriptPosition  `json:"end"`
}

// CallFrame JavaScript call frame. Array of call frames form the call stack.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-CallFrame
type CallFrame struct {
	CallFrameID      CallFrameID           `json:"callFrameId"`                // Call frame identifier. This identifier is only valid while the virtual machine is paused.
	FunctionName     string                `json:"functionName"`               // Name of the JavaScript function called on this call frame.
	FunctionLocation *Location             `json:"functionLocation,omitempty"` // Location in the source code.
	Location         *Location             `json:"location"`                   // Location in the source code.
	ScopeChain       []*Scope              `json:"scopeChain"`                 // Scope chain for this call frame.
	This             *runtime.RemoteObject `json:"this"`                       // this object for this call frame.
	ReturnValue      *runtime.RemoteObject `json:"returnValue,omitempty"`      // The value being returned, if the function is at return point.
	CanBeRestarted   bool                  `json:"canBeRestarted,omitempty"`   // Valid only while the VM is paused and indicates whether this frame can be restarted or not. Note that a true value here does not guarantee that Debugger#restartFrame with this CallFrameId will be successful, but it is very likely.
}

// Scope scope description.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-Scope
type Scope struct {
	Type          ScopeType             `json:"type"`   // Scope type.
	Object        *runtime.RemoteObject `json:"object"` // Object representing the scope. For global and with scopes it represents the actual object; for the rest of the scopes, it is artificial transient object enumerating scope variables as its properties.
	Name          string                `json:"name,omitempty"`
	StartLocation *Location             `json:"startLocation,omitempty"` // Location in the source code where scope starts
	EndLocation   *Location             `json:"endLocation,omitempty"`   // Location in the source code where scope ends
}

// SearchMatch search match for resource.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-SearchMatch
type SearchMatch struct {
	LineNumber  float64 `json:"lineNumber"`  // Line number in resource content.
	LineContent string  `json:"lineContent"` // Line with match content.
}

// BreakLocation [no description].
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-BreakLocation
type BreakLocation struct {
	ScriptID     runtime.ScriptID  `json:"scriptId"`               // Script identifier as reported in the Debugger.scriptParsed.
	LineNumber   int64             `json:"lineNumber"`             // Line number in the script (0-based).
	ColumnNumber int64             `json:"columnNumber,omitempty"` // Column number in the script (0-based).
	Type         BreakLocationType `json:"type,omitempty"`
}

// WasmDisassemblyChunk [no description].
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-WasmDisassemblyChunk
type WasmDisassemblyChunk struct {
	Lines           []string `json:"lines"`           // The next chunk of disassembled lines.
	BytecodeOffsets []int64  `json:"bytecodeOffsets"` // The bytecode offsets describing the start of each line.
}

// ScriptLanguage enum of possible script languages.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-ScriptLanguage
type ScriptLanguage string

// String returns the ScriptLanguage as string value.
func (t ScriptLanguage) String() string {
	return string(t)
}

// ScriptLanguage values.
const (
	ScriptLanguageJavaScript  ScriptLanguage = "JavaScript"
	ScriptLanguageWebAssembly ScriptLanguage = "WebAssembly"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t ScriptLanguage) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t ScriptLanguage) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *ScriptLanguage) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch ScriptLanguage(v) {
	case ScriptLanguageJavaScript:
		*t = ScriptLanguageJavaScript
	case ScriptLanguageWebAssembly:
		*t = ScriptLanguageWebAssembly

	default:
		in.AddError(fmt.Errorf("unknown ScriptLanguage value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *ScriptLanguage) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// DebugSymbols debug symbols available for a wasm script.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-DebugSymbols
type DebugSymbols struct {
	Type        DebugSymbolsType `json:"type"`                  // Type of the debug symbols.
	ExternalURL string           `json:"externalURL,omitempty"` // URL of the external symbol source.
}

// ScopeType scope type.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-Scope
type ScopeType string

// String returns the ScopeType as string value.
func (t ScopeType) String() string {
	return string(t)
}

// ScopeType values.
const (
	ScopeTypeGlobal              ScopeType = "global"
	ScopeTypeLocal               ScopeType = "local"
	ScopeTypeWith                ScopeType = "with"
	ScopeTypeClosure             ScopeType = "closure"
	ScopeTypeCatch               ScopeType = "catch"
	ScopeTypeBlock               ScopeType = "block"
	ScopeTypeScript              ScopeType = "script"
	ScopeTypeEval                ScopeType = "eval"
	ScopeTypeModule              ScopeType = "module"
	ScopeTypeWasmExpressionStack ScopeType = "wasm-expression-stack"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t ScopeType) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t ScopeType) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *ScopeType) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch ScopeType(v) {
	case ScopeTypeGlobal:
		*t = ScopeTypeGlobal
	case ScopeTypeLocal:
		*t = ScopeTypeLocal
	case ScopeTypeWith:
		*t = ScopeTypeWith
	case ScopeTypeClosure:
		*t = ScopeTypeClosure
	case ScopeTypeCatch:
		*t = ScopeTypeCatch
	case ScopeTypeBlock:
		*t = ScopeTypeBlock
	case ScopeTypeScript:
		*t = ScopeTypeScript
	case ScopeTypeEval:
		*t = ScopeTypeEval
	case ScopeTypeModule:
		*t = ScopeTypeModule
	case ScopeTypeWasmExpressionStack:
		*t = ScopeTypeWasmExpressionStack

	default:
		in.AddError(fmt.Errorf("unknown ScopeType value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *ScopeType) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// BreakLocationType [no description].
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-BreakLocation
type BreakLocationType string

// String returns the BreakLocationType as string value.
func (t BreakLocationType) String() string {
	return string(t)
}

// BreakLocationType values.
const (
	BreakLocationTypeDebuggerStatement BreakLocationType = "debuggerStatement"
	BreakLocationTypeCall              BreakLocationType = "call"
	BreakLocationTypeReturn            BreakLocationType = "return"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t BreakLocationType) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t BreakLocationType) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *BreakLocationType) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch BreakLocationType(v) {
	case BreakLocationTypeDebuggerStatement:
		*t = BreakLocationTypeDebuggerStatement
	case BreakLocationTypeCall:
		*t = BreakLocationTypeCall
	case BreakLocationTypeReturn:
		*t = BreakLocationTypeReturn

	default:
		in.AddError(fmt.Errorf("unknown BreakLocationType value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *BreakLocationType) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// DebugSymbolsType type of the debug symbols.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#type-DebugSymbols
type DebugSymbolsType string

// String returns the DebugSymbolsType as string value.
func (t DebugSymbolsType) String() string {
	return string(t)
}

// DebugSymbolsType values.
const (
	DebugSymbolsTypeSourceMap     DebugSymbolsType = "SourceMap"
	DebugSymbolsTypeEmbeddedDWARF DebugSymbolsType = "EmbeddedDWARF"
	DebugSymbolsTypeExternalDWARF DebugSymbolsType = "ExternalDWARF"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t DebugSymbolsType) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t DebugSymbolsType) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *DebugSymbolsType) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch DebugSymbolsType(v) {
	case DebugSymbolsTypeSourceMap:
		*t = DebugSymbolsTypeSourceMap
	case DebugSymbolsTypeEmbeddedDWARF:
		*t = DebugSymbolsTypeEmbeddedDWARF
	case DebugSymbolsTypeExternalDWARF:
		*t = DebugSymbolsTypeExternalDWARF

	default:
		in.AddError(fmt.Errorf("unknown DebugSymbolsType value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *DebugSymbolsType) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// PausedReason pause reason.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#event-paused
type PausedReason string

// String returns the PausedReason as string value.
func (t PausedReason) String() string {
	return string(t)
}

// PausedReason values.
const (
	PausedReasonAmbiguous        PausedReason = "ambiguous"
	PausedReasonAssert           PausedReason = "assert"
	PausedReasonCSPViolation     PausedReason = "CSPViolation"
	PausedReasonDebugCommand     PausedReason = "debugCommand"
	PausedReasonDOM              PausedReason = "DOM"
	PausedReasonEventListener    PausedReason = "EventListener"
	PausedReasonException        PausedReason = "exception"
	PausedReasonInstrumentation  PausedReason = "instrumentation"
	PausedReasonOOM              PausedReason = "OOM"
	PausedReasonOther            PausedReason = "other"
	PausedReasonPromiseRejection PausedReason = "promiseRejection"
	PausedReasonXHR              PausedReason = "XHR"
	PausedReasonStep             PausedReason = "step"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t PausedReason) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t PausedReason) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *PausedReason) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch PausedReason(v) {
	case PausedReasonAmbiguous:
		*t = PausedReasonAmbiguous
	case PausedReasonAssert:
		*t = PausedReasonAssert
	case PausedReasonCSPViolation:
		*t = PausedReasonCSPViolation
	case PausedReasonDebugCommand:
		*t = PausedReasonDebugCommand
	case PausedReasonDOM:
		*t = PausedReasonDOM
	case PausedReasonEventListener:
		*t = PausedReasonEventListener
	case PausedReasonException:
		*t = PausedReasonException
	case PausedReasonInstrumentation:
		*t = PausedReasonInstrumentation
	case PausedReasonOOM:
		*t = PausedReasonOOM
	case PausedReasonOther:
		*t = PausedReasonOther
	case PausedReasonPromiseRejection:
		*t = PausedReasonPromiseRejection
	case PausedReasonXHR:
		*t = PausedReasonXHR
	case PausedReasonStep:
		*t = PausedReasonStep

	default:
		in.AddError(fmt.Errorf("unknown PausedReason value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *PausedReason) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// ContinueToLocationTargetCallFrames [no description].
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#method-continueToLocation
type ContinueToLocationTargetCallFrames string

// String returns the ContinueToLocationTargetCallFrames as string value.
func (t ContinueToLocationTargetCallFrames) String() string {
	return string(t)
}

// ContinueToLocationTargetCallFrames values.
const (
	ContinueToLocationTargetCallFramesAny     ContinueToLocationTargetCallFrames = "any"
	ContinueToLocationTargetCallFramesCurrent ContinueToLocationTargetCallFrames = "current"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t ContinueToLocationTargetCallFrames) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t ContinueToLocationTargetCallFrames) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *ContinueToLocationTargetCallFrames) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch ContinueToLocationTargetCallFrames(v) {
	case ContinueToLocationTargetCallFramesAny:
		*t = ContinueToLocationTargetCallFramesAny
	case ContinueToLocationTargetCallFramesCurrent:
		*t = ContinueToLocationTargetCallFramesCurrent

	default:
		in.AddError(fmt.Errorf("unknown ContinueToLocationTargetCallFrames value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *ContinueToLocationTargetCallFrames) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// RestartFrameMode the mode parameter must be present and set to 'StepInto',
// otherwise restartFrame will error out.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#method-restartFrame
type RestartFrameMode string

// String returns the RestartFrameMode as string value.
func (t RestartFrameMode) String() string {
	return string(t)
}

// RestartFrameMode values.
const (
	RestartFrameModeStepInto RestartFrameMode = "StepInto"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t RestartFrameMode) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t RestartFrameMode) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *RestartFrameMode) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch RestartFrameMode(v) {
	case RestartFrameModeStepInto:
		*t = RestartFrameModeStepInto

	default:
		in.AddError(fmt.Errorf("unknown RestartFrameMode value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *RestartFrameMode) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// SetInstrumentationBreakpointInstrumentation instrumentation name.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#method-setInstrumentationBreakpoint
type SetInstrumentationBreakpointInstrumentation string

// String returns the SetInstrumentationBreakpointInstrumentation as string value.
func (t SetInstrumentationBreakpointInstrumentation) String() string {
	return string(t)
}

// SetInstrumentationBreakpointInstrumentation values.
const (
	SetInstrumentationBreakpointInstrumentationBeforeScriptExecution              SetInstrumentationBreakpointInstrumentation = "beforeScriptExecution"
	SetInstrumentationBreakpointInstrumentationBeforeScriptWithSourceMapExecution SetInstrumentationBreakpointInstrumentation = "beforeScriptWithSourceMapExecution"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t SetInstrumentationBreakpointInstrumentation) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t SetInstrumentationBreakpointInstrumentation) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *SetInstrumentationBreakpointInstrumentation) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch SetInstrumentationBreakpointInstrumentation(v) {
	case SetInstrumentationBreakpointInstrumentationBeforeScriptExecution:
		*t = SetInstrumentationBreakpointInstrumentationBeforeScriptExecution
	case SetInstrumentationBreakpointInstrumentationBeforeScriptWithSourceMapExecution:
		*t = SetInstrumentationBreakpointInstrumentationBeforeScriptWithSourceMapExecution

	default:
		in.AddError(fmt.Errorf("unknown SetInstrumentationBreakpointInstrumentation value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *SetInstrumentationBreakpointInstrumentation) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// ExceptionsState pause on exceptions mode.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#method-setPauseOnExceptions
type ExceptionsState string

// String returns the ExceptionsState as string value.
func (t ExceptionsState) String() string {
	return string(t)
}

// ExceptionsState values.
const (
	ExceptionsStateNone     ExceptionsState = "none"
	ExceptionsStateCaught   ExceptionsState = "caught"
	ExceptionsStateUncaught ExceptionsState = "uncaught"
	ExceptionsStateAll      ExceptionsState = "all"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t ExceptionsState) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t ExceptionsState) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *ExceptionsState) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch ExceptionsState(v) {
	case ExceptionsStateNone:
		*t = ExceptionsStateNone
	case ExceptionsStateCaught:
		*t = ExceptionsStateCaught
	case ExceptionsStateUncaught:
		*t = ExceptionsStateUncaught
	case ExceptionsStateAll:
		*t = ExceptionsStateAll

	default:
		in.AddError(fmt.Errorf("unknown ExceptionsState value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *ExceptionsState) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}

// SetScriptSourceStatus whether the operation was successful or not. Only Ok
// denotes a successful live edit while the other enum variants denote why the
// live edit failed.
//
// See: https://chromedevtools.github.io/devtools-protocol/tot/Debugger#method-setScriptSource
type SetScriptSourceStatus string

// String returns the SetScriptSourceStatus as string value.
func (t SetScriptSourceStatus) String() string {
	return string(t)
}

// SetScriptSourceStatus values.
const (
	SetScriptSourceStatusOk                              SetScriptSourceStatus = "Ok"
	SetScriptSourceStatusCompileError                    SetScriptSourceStatus = "CompileError"
	SetScriptSourceStatusBlockedByActiveGenerator        SetScriptSourceStatus = "BlockedByActiveGenerator"
	SetScriptSourceStatusBlockedByActiveFunction         SetScriptSourceStatus = "BlockedByActiveFunction"
	SetScriptSourceStatusBlockedByTopLevelEsModuleChange SetScriptSourceStatus = "BlockedByTopLevelEsModuleChange"
)

// MarshalEasyJSON satisfies easyjson.Marshaler.
func (t SetScriptSourceStatus) MarshalEasyJSON(out *jwriter.Writer) {
	out.String(string(t))
}

// MarshalJSON satisfies json.Marshaler.
func (t SetScriptSourceStatus) MarshalJSON() ([]byte, error) {
	return easyjson.Marshal(t)
}

// UnmarshalEasyJSON satisfies easyjson.Unmarshaler.
func (t *SetScriptSourceStatus) UnmarshalEasyJSON(in *jlexer.Lexer) {
	v := in.String()
	switch SetScriptSourceStatus(v) {
	case SetScriptSourceStatusOk:
		*t = SetScriptSourceStatusOk
	case SetScriptSourceStatusCompileError:
		*t = SetScriptSourceStatusCompileError
	case SetScriptSourceStatusBlockedByActiveGenerator:
		*t = SetScriptSourceStatusBlockedByActiveGenerator
	case SetScriptSourceStatusBlockedByActiveFunction:
		*t = SetScriptSourceStatusBlockedByActiveFunction
	case SetScriptSourceStatusBlockedByTopLevelEsModuleChange:
		*t = SetScriptSourceStatusBlockedByTopLevelEsModuleChange

	default:
		in.AddError(fmt.Errorf("unknown SetScriptSourceStatus value: %v", v))
	}
}

// UnmarshalJSON satisfies json.Unmarshaler.
func (t *SetScriptSourceStatus) UnmarshalJSON(buf []byte) error {
	return easyjson.Unmarshal(buf, t)
}
